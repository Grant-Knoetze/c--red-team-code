// API encryption example
// Using the MSDN API definition
// Credit to Amr Thabet from Maltrak - The eternal mentor.


// Define MSGBOXDEF as a function that returns an int and takes four arguments

#define MSGBOXDEF int (_stdcall *)(HWND, LPCSTR, LPCSTR, UINT)

// Create a variable called MessageBoxAPI with the same arguments as follows

int (_stdcall *MessageBoxAPI)(
     HWND   hWnd,
	 LPCSTR lpText,
	 LPCSTR lpCaption,
	 UINT   uType
	 );
	 
int MessageBoxDynamicallyLoaded()
{

   unsigned char* enc_str = (unsigned char*)""; // encrypted string
   int length = strlen((char*)enc_str);
   MessageBoxAPI = (MSGBOXDEF)GetProcAddress(LoadLibrary("User32.dll"), (char*)RC4((char*)"1234", (unsigned char*)enc_str, length));
   (*MessageBoxAPI)(0, "Hello Dynamic", "MART Agent", 0);
   return 0;

}

