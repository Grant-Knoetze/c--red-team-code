//Header file containing the Windows API's and libraries necessary for HTTP GET/POST functionality.


#pragma once
#include <iostream>
#include <windows.h>
#include <wininet.h>
#pragma comment (lib, "Wininet.lib")

HINTERNET SendRequest(LPCSTR Domain, int Port, LPCSTR URI, LPCSTR Verb, const char* Params);
