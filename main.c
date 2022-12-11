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

    //printf("connect socket\n");

    if(login(&ftp, user, pw) == -1) {
        return -1;
    }

    //printf("login\n");

    if(cwd(&ftp, components.path) == -1) {
        return -1;
    }

    //printf("cwd\n");

    //if(getFileSize(&ftp, "timestamp.txt\n") == -1) {
    //    return -1;
    //}

    //printf("get file size\n");

    if(passiveMode(&ftp) == -1) {
        return -1;
    }

    //printf("passive mode\n");

    if(retrieve(&ftp, components.filename) == -1) {
        return -1;
    }

    //printf("retrieve\n");

    if(download(&ftp, components.filename) == -1) {
        return -1;
    }

    //printf("download\n");

    if(disconnect(&ftp) == -1) {
        printf("Failed to disconnect\n");
    }

    return 0;
}
