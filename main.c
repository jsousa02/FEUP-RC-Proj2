#include <stdio.h>

#include "client.h"
#include "parser.h"

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    url_parts components;

    initParser(&components);

    if(parseURL(argv[1], &components)) {
        printf("Failed to parse URL\n");
        return -1;
    }

    printf("Host: %s\nPath: %s\nFilename: %s\nUser: %s\nPassword: %s\n", components.host, components.path, components.filename, components.user, components.password);

    ftp_info ftp;

    getip(&components);
    printf("IP: %s\n", components.ip);

    if(connectSocket(&ftp, components.ip, components.port) == -1) {
        return -1;
    }

    const char* user = strlen(components.user) ? components.user : "anonymous";
    
    const char* pw = strlen(components.password) ? components.password: "anonymous@";

    if(login(&ftp, user, pw) == -1) {
        return -1;
    }

    if(cwd(&ftp, components.path) == -1) {
        return -1;
    }

    if(passiveMode(&ftp) == -1) {
        return -1;
    }

    if(retrieve(&ftp, components.filename) == -1) {
        return -1;
    }

    if(download(&ftp, components.filename) == -1) {
        return -1;
    }

    if(disconnect(&ftp) == -1) {
        printf("Failed to disconnect\n");
    }

    return 0;
}
