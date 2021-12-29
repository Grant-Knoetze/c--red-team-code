// RedTeamAgent.cpp
// This agent opens up an HTTP session and transmits data to a URL

#include "http.h"

const char* Params = ""; // Data for transmission
int main()
{
	HINTERNET hSession = SendRequest(/*URL*/, 80, "/beacon", "POST", Params);
	if (hSession != NULL)
	{
		printf("Data has been sent successfully\n");
	}
	else
	{
		printf("Error in sending data\n");
	}
}



