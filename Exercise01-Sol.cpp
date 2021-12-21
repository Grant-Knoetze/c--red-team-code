// Module04Exercise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


void EncryptString(char* str, int length, char* buf, char key)
{
	for (int i = 0; i < length; i++)
	{
		buf[i] = str[i] ^ key;
	}
}
int main()
{
	char* str = (char*) "http://192.168.1.3:8888/beacon"; 
	int length = strlen(str);
	char* buf = (char*)malloc(length);

	// your encryption here
	EncryptString(str, length, buf, 0x01);

	printf("Encrypted String: %s", buf);

}
