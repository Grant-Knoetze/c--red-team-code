#include "execute.h"


std::wstring ConvertAnsiToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}

//Remove all bytes thare aren't from the first 127
string RemoveNonLetters(string& input)
{
	for (int i = 0; i < input.length(); i++)
	{
		if ((unsigned char)input.c_str()[i] > 127)
		{
			printf("%d\n", (unsigned char)input.c_str()[i]);
			input.erase(i, 1);
		};
	};
	return input;
}

char* CopyStr(char* src, int length)
{
	//make sure it's a null terminated string
	char* dest = (char*)malloc(length + 1);
	if (dest == NULL)
		return NULL;
	memset(dest, 0, length + 1);
	memcpy(dest, src, length);
	return dest;
}


//30 secs
#define MAX_WAITTIME 100 * 30 
char* ExecuteApplication(string cmd, size_t& OutputLength)
{
	char buf[1024 + 1] = { };
	DWORD bytesRead = 0;
	char* output = NULL;
	std::string result = "";

	BOOL ok = TRUE;
	HANDLE hStdInPipeRead = NULL;
	HANDLE hStdInPipeWrite = NULL;
	HANDLE hStdOutPipeRead = NULL;
	HANDLE hStdOutPipeWrite = NULL;

	// Create two pipes.
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	ok = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
	ok = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);

	// Create the process.
	STARTUPINFO si = { };
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdError = hStdOutPipeWrite;
	si.hStdOutput = hStdOutPipeWrite;
	si.hStdInput = hStdInPipeRead;

	PROCESS_INFORMATION pi = { };
	LPSTR lpCommandLine = (LPSTR)cmd.c_str();
	ok = CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	if (ok == FALSE) return NULL;

	WaitForSingleObject(pi.hProcess, MAX_WAITTIME);
	TerminateProcess(pi.hProcess, 0);

	// Close pipes we do not need.
	CloseHandle(hStdOutPipeWrite);
	CloseHandle(hStdInPipeRead);

	// The main loop for reading output from the DIR command.
	DWORD dwRead = 0;
	DWORD dwAvail = 0;
	ok = ReadFile(hStdOutPipeRead, buf, 1024, &dwRead, NULL);
	while (ok == TRUE)
	{
		buf[dwRead] = '\0';
		result += buf;
		ok = ReadFile(hStdOutPipeRead, buf, 1024, &dwRead, NULL);
	}
	// Clean up and exit.
	CloseHandle(hStdOutPipeRead);
	CloseHandle(hStdInPipeWrite);

	RemoveNonLetters(result);
	OutputLength = result.length();
	if (result.length() == 0)
		return NULL;
	output = CopyStr((char*)result.c_str(), result.length());
	return output;
}
