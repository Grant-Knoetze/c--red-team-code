#include "http.h"

LPCSTR UserAgent = ""; // Insert user agent here

 HINTERNET SendRequest(LPCSTR Domain, int Port, LPCSTR URI, LPCSTR Verb, const char* Params)
{ 
     // This code is using the Windows InternetOpen and InternetConnect to establish an HTTP connection.

    HINTERNET InternetHandle = InternetOpenA(UserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET ConnectHandle = InternetConnectA(InternetHandle, Domain, Port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
    if (ConnectHandle == NULL)
    {
        return NULL;
    }
    HINTERNET hRequest = HttpOpenRequestA(ConnectHandle, Verb, URI, "HTTP/1.1", NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION, NULL);
    if (HttpSendRequest(hRequest, NULL, 0, (LPVOID)Params, strlen(Params)))
    {
        return hRequest;
    }
    else
    {
        return NULL;
    }
}   


