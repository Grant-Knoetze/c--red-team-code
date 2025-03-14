int MessageBoxDynamicallyLoaded()
{
    unsigned char* enc_str = (unsigned char*)"";
    int length = strlen((char*)enc_str);
    MessageBoxAPI = (MSGBOXDEF)GetProcAddress(LoadLibrary("User32.dll"), (char*)RC4((char*)"1234", (unsigned char*)enc_str, length));
    (*MessageBoxAPI)(0, "Hello Dynamic", "MART Agent", 0);
    return 0;
}