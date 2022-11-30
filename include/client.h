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

int openSocket(const char* ip, int port);
int connectSocket();
int sendCommand(char* buf, size_t size);
int readResponse(char* buf, size_t size);
int getip(const char* hostname);
int login(const char* user, const char* password);
int cwd(const char* path);
int getFileSize(const char* filename);
int passiveMode();
int retrieve(const char* filename);
int download(const char* filename);
int closeSocket();

#endif
