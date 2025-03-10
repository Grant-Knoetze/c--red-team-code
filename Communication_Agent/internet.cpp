#include "internet.h"

#pragma comment (lib, "Wininet.lib")

//Important Strings
LPCSTR UserAgent = "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; InfoPath.1)";
LPCSTR acceptTypes[] = { "application/x-www-form-urlencoded", NULL };


HINTERNET SendRequest(LPCSTR Domain, int Port, LPCSTR URI, LPCSTR Verb, const char* Params)
{
	HINTERNET hSession, hConnect, hFile;
	if ((hSession = InternetOpen(UserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)) == NULL)
		return NULL;
	if ((hConnect = InternetConnect(hSession, Domain, Port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0)) == NULL)
		return NULL;

	if ((hFile = HttpOpenRequest(hConnect, Verb, URI, NULL, NULL, acceptTypes, INTERNET_FLAG_RELOAD, 0)) == NULL)
		return NULL;

	unsigned long dataLen = strlen(Params);
	if (HttpSendRequest(hFile, NULL, 0, (char*)Params, dataLen) == TRUE)
		return hFile;
	else
		return NULL;
}

int GetStatusCode(HINTERNET hFile)
{
	int statusCode = 0;
	DWORD nStatusCodeLength = sizeof(statusCode);

	if (!HttpQueryInfo(hFile,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		&statusCode,
		&nStatusCodeLength,
		NULL))
		return 0;
	return statusCode;
}

char* GetResponse(HINTERNET hFile, DWORD* Length)
{
	DWORD ContentLength = 0;
	DWORD varLength = sizeof(DWORD);
	DWORD dwIndex;
	dwIndex = 0;
	*Length = 0;
	// get Content-Length value
	if (!HttpQueryInfo(
		hFile,
		HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
		&ContentLength,
		&varLength,
		&dwIndex))
		return NULL;

	*Length = ContentLength;
	//Now time to grab the content
	char* Buffer = (char*)malloc(ContentLength + 1);
	memset(Buffer, 0, ContentLength + 1);
	if (InternetReadFile(hFile, Buffer, ContentLength, &dwIndex))
		return Buffer;
	else
		return NULL;
}

//FileToDownload: Only Add FileToDownload when downloading from Caldera to add the extra header "file:<filename>"
char* DownloadFile(LPCSTR URL, DWORD* FileLength, LPCSTR FileToDownload = NULL)
{
	HINTERNET hSession, hURL;
	BOOL bResult;
	DWORD dwBytesRead = 1;


	hSession = InternetOpen(
		UserAgent, // agent
		INTERNET_OPEN_TYPE_PRECONFIG, 
		NULL, NULL, 0);

	char* FileHeader = NULL;
	int FileHeaderLength = 0;

	if (FileToDownload)
	{
		FileHeaderLength = snprintf(NULL, 0, "file:%s", FileToDownload);
		FileHeader = (char*)malloc(FileHeaderLength + 1);
		memset(FileHeader, 0, FileHeaderLength + 1);
		snprintf(FileHeader, FileHeaderLength + 1, "file:%s", FileToDownload);
	}
	
	hURL = InternetOpenUrl(
		hSession,   
		URL,
		FileHeader, FileHeaderLength,	// File Header
		0, 0);							// defaults

	
	DWORD Length = 0;
	char* FileData = GetResponse(hURL, &Length);
	*FileLength = Length;
	InternetCloseHandle(hURL);
	InternetCloseHandle(hSession);
	return FileData;
}
