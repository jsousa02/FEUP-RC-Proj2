#include <stdio.h>
#include "include/client.h"

int main(int argc, char** argv) {

    if(openSocket("193.137.29.15", 50826) == -1) {
        return -1;
    }

    if(connectSocket() == -1) {
        return -1;
    }

    if(login("anonymous", "anonymous@") == -1) {
        return -1;
    }

    if(cwd("/pub/kodi") == -1) {
        return -1;
    }

    if(getFileSize("timestamps.txt\n") == -1) {
        return -1;
    }

    if(passiveMode() == -1) {
        return -1;
    }

    if(retrieve("timestamps.txt\n") == -1) {
        return -1;
    }

    if(download("timestamps.txt\n") == -1) {
        return -1;
    }

    if(closeSocket() == -1) {
        return -1;
    }

    return 0;
}
