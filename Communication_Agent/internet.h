#include <windows.h>
#include <stdio.h> 
#include <wininet.h>
#include <string>
#include <winternl.h>

using namespace std;

//Internet Communication
HINTERNET SendRequest(LPCSTR Domain, int Port, LPCSTR URI, LPCSTR Verb, const char* Params);
int GetStatusCode(HINTERNET hFile);
char* GetResponse(HINTERNET hFile, DWORD* Length);
char* DownloadFile(LPCSTR URL, DWORD* FileLength, LPCSTR FileToDownload);
