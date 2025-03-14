//Encrypt a string using XOR (represented by a caret ^ in C++)
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
	char* str = (char*) "http://127.0.0.1:8888/beacon"; // Example string to encrypt
	int length = strlen(str);
	char* buf = (char*)malloc(length);

	// your encryption here
	EncryptString(str, length, buf, 0x01);

	printf("Encrypted String: %s", buf);

	return 0;

}
