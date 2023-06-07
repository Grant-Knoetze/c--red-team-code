// SSH_C&C.cpp : This file contains the 'main' function. Program execution begins and ends there.
// You will need to install libssh https://www.libssh.org/get-it/
// Establish a simple SSH session.
#include <libssh/libssh.h>
#include <iostream>

int main()
{
    ssh_session my_ssh_session;
    int rc;
    const char* password;
    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL) // Exit if session is null
        exit(-1);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "127.0.0.1");
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, "900");
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "c2user");
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to localhost: %s\n",
            ssh_get_error(my_ssh_session));
        ssh_free(my_ssh_session);
        exit(-1);
        password = ("Password");
        rc = ssh_userauth_password(my_ssh_session, NULL, password);
        if (rc != SSH_AUTH_SUCCESS)
        {
            fprintf(stderr, "Error authenticating with password: %s\n",
                ssh_get_error(my_ssh_session));
            ssh_disconnect(my_ssh_session);
            ssh_free(my_ssh_session);
            exit(-1);
        }
    }
}

