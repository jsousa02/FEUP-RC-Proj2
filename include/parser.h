#ifndef PARSER_H_
#define PARSER_H_

#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 256

typedef struct {
    char filename[SIZE];
    char user[SIZE];
    char password[SIZE];
    char host[SIZE];
    char path[SIZE];
    char ip[SIZE];
    int port;
} url_parts;

void initParser(url_parts* url);
int parseURL(char* url, url_parts* components);
char* cutString(char* str, char chr);

#endif
