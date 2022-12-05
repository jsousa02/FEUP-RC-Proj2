#include <stdio.h>
#include "include/client.h"

int main(int argc, char** argv) {

    ftp_info ftp;
    int sockfd;

    if((sockfd = openSocket(&ftp, "193.137.29.15", 50826)) == -1) {
        return -1;
    }
    printf("open socket\n");

    if(connectSocket(&ftp, sockfd) == -1) {
        return -1;
    }

    printf("connect socket\n");

    if(login(&ftp, "anonymous", "anonymous@") == -1) {
        return -1;
    }

    printf("login\n");

    if(cwd(&ftp, "/pub/kodi") == -1) {
        return -1;
    }

    printf("cwd\n");

    if(getFileSize(&ftp, "timestamps.txt\n") == -1) {
        return -1;
    }

    printf("get file size\n");

    if(passiveMode(&ftp) == -1) {
        return -1;
    }

    printf("passive mode\n");

    if(retrieve(&ftp, "timestamps.txt\n") == -1) {
        return -1;
    }

    printf("retrieve\n");

    if(download(&ftp, "timestamps.txt\n") == -1) {
        return -1;
    }

    printf("download\n");

    if(closeSocket(&ftp) == -1) {
        return -1;
    }

    printf("close socket\n");

    return 0;
}
