#include <stdio.h> 
#include <Windows.h>
#include <string>
using namespace std;


//String Manipulation
char* ExtractString(string src, char* start, char* end, int& pos);
wstring ConvertAnsiToWide(const string& str);

//Executing Applications
char* ExecuteApplication(string cmd, size_t& OutputLength);