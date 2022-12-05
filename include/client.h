#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

typedef struct {
    int control_socket;
    int data_socket;
    struct sockaddr_in server_addr;
    struct hostent* h;
    int host;
    int port;
} ftp_info;

int openSocket(ftp_info* ftp, const char* ip, int port);
int connectSocket(ftp_info* ftp, int socketfd);
int sendCommand(ftp_info* ftp, char* buf, size_t size);
int readResponse(ftp_info* ftp, char* buf, size_t size);
int getip(ftp_info* ftp, const char* hostname);
int login(ftp_info* ftp, const char* user, const char* password);
int cwd(ftp_info* ftp, const char* path);
int getFileSize(ftp_info* ftp, const char* filename);
int passiveMode(ftp_info* ftp);
int retrieve(ftp_info* ftp, const char* filename);
int download(ftp_info* ftp, const char* filename);
int closeSocket(ftp_info* ftp);

#endif
