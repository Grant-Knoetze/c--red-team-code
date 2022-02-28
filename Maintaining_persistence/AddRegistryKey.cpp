int main()
{
    
   // Just inside the main function, add the following code to create a new registry key in HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run 

    UpdateRegistry(HKEY_CURRENT_USER, (char*)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", (char*)"RedTeamAgentNew", (char*)"C:\\RedTeamAgent.exe", strlen("C:\\RedTeamAgent.exe"), REG_SZ, false);
    return 0;