#include "client.h"

ftp_info* ftp;

int openSocket(const char* ip, int port) {
    int sockfd;

    struct sockaddr_in server_addr;

    bzero(&server_addr, sizeof(server_addr));
    ftp->server_addr.sin_family = AF_INET;
    ftp->server_addr.sin_addr.s_addr = inet_addr(ip);
    ftp->server_addr.sin_port = htons(port);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    return sockfd;
}

int connectSocket(int socketfd) {
    if(connect(socketfd, (struct sockaddr*) &ftp->server_addr, sizeof(ftp->server_addr)) < 0) {
        perror("connect()");
        return -1;
    }

    return 0;
}

int sendCommand(char* buf, size_t size) {
    int bytes = write(ftp->control_socket, buf, size);

    return bytes == size;
}

int readResponse(char* buf, size_t size) {
    FILE* file = fdopen(ftp->control_socket, "r");

    do {
        memset(buf, 0, size);
        fgets(buf, size, file);
    
    } while(buf[3] != ' ');

    return 0;
}

int getip(const char* hostname) {

    if((ftp->h = gethostbyname(hostname)) == NULL) {
        herror("gethostbyname()");
        return -1;
    }

    return 0;
}

int login(const char* user, const char* password) {
    char buf[1024];

    sprintf(buf, "USER %s\n", user);

    if(sendCommand(buf, 1024) == -1) {
        printf("Failed to send user\n");
        return -1;
    }

    if(readResponse(buf, 1024) == -1) {
        printf("Failed to read user response\n");
        return -1;
    }

    memset(buf, 0, 1024);

    sprintf(buf, "PASS %s\n", password);

    if(sendCommand(buf, 1024) == -1) {
        printf("Failed to send password\n");
        return -1;
    }

    if(readResponse(buf, 1024) == -1) {
        printf("Failed to read password response\n");
        return -1;
    }

    return 0;
}

int cwd(const char* path) {
    char buf[1024];
    sprintf(buf, "CWD %s\n", path);

    if(sendCommand(buf, 1024) == -1) {
        printf("Failed to change current working directory\n");
        return -1;
    }

    if(readResponse(buf, 1024) == -1) {
        printf("Failed to read cwd response\n");
        return -1;
    }

    return 0;
}

int getFileSize(const char* filename) {
    char binary_mode[] = "TYPE I\n";
    char file_size[1024];
    char response[1024];

    sprintf(file_size, "SIZE %s\n", filename);

    if(sendCommand(binary_mode, strlen(binary_mode)) == -1) {
        printf("Failed to switch to binary mode\n");
        return -1;
    }

    if(readResponse(response, 1024) == -1) {
        printf("Failed to read switch to binary mode response\n");
        return -1;
    }

    if(sendCommand(file_size, 1024) == -1) {
        printf("Failed to read get file size response\n");
        return -1;
    }

    return 0;
}

int passiveMode() {
    char buf[] = "PASV\n";
    char response[1024];
    char ip[] = "\0"; 

    int ip1, ip2, ip3, ip4, port1, port2;

    if(sendCommand(buf, strlen(buf)) == -1) {
        printf("Failed to enter passive mode\n");
        return -1;
    }

    if(readResponse(response, sizeof(response)) == -1) {
        printf("Failed to read passive mode response\n");
        return -1;
    }

    if((sscanf(response, "227 Entering Passive Mode (%d,%d,%d,%d,%d).", &ip1, &ip2, &ip3, &ip4, &port1, &port2)) < 0)
    {
            printf("Failed to get IP address\n");
            return -1;
    }

    if((sprintf(ip, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4)) < 0) {
        printf("Failed to format ip address\n");
        return -1;
    }

    int port = 256 * port1 + port2;

    ftp->data_socket = openSocket(ip, port);

    return 0;
}

int retrieve(const char* filename) {
    char buf[1024];
    char response[1024];
    sprintf(buf, "RETR %s\n", filename);

    if(sendCommand(buf, sizeof(buf)) == -1) {
        printf("Failed to send RETR command\n");
        return -1;
    }

    if(readResponse(response, sizeof(response)) == -1) {
        printf("Failed to read RETR response\n");
        return -1;
    }

    return 0;
}

int download(const char* filename) {
    FILE* file = fopen(filename, "w");
    int bytes, bytesWritten;

    if(!file) {
        printf("Failed to open file\n");
        return -1;
    }

    char buf[1024];

    while(bytes = read(ftp->data_socket, buf, sizeof(buf))) {
        if(bytes < 0) {
            printf("Failed to read file content\n");
            return -1;
        }

        if((bytesWritten = fwrite(buf, bytes, 1, file)) < 0) {
            printf("Failed to write file content\n");
            return -1;
        }
    }

    fclose(file);
    close(ftp->data_socket);

    return 0;
}

int closeSocket() {
    return close(ftp->control_socket);
}
