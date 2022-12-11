#include "parser.h"

void initParser(url_parts* url) {
    memset(url->filename, 0, SIZE);
    memset(url->host, 0, SIZE);
    memset(url->password, 0, SIZE);
    memset(url->path, 0, SIZE);
    memset(url->user, 0, SIZE);
    url->port = 21;
}

int parseURL(char* url, url_parts* components) {
    printf("url: %s\n", url);

    const char* authExpr = "ftp://([([A-Za-z0-9])*:([A-Za-z0-9])*@])*([A-Za-z0-9.~-])+/([[A-Za-z0-9/~._-])+";
    const char* expre = "ftp://([A-Za-z0-9.~-])+/([[A-Za-z0-9/~._-])+";

    char* expr = (char*) expre;
    char* tmp = (char*) malloc(strlen(url));
    char* aux = (char*) malloc(strlen(url));
    memcpy(tmp, url, strlen(url));

    size_t nmatch = strlen(url);
    regmatch_t pmatch[nmatch];

    int auth = 0;

    if(url[6] == '[') {
        auth = 1;
        expr = (char*) authExpr;
    }

    regex_t* reegex = (regex_t*) malloc(sizeof(regex_t));
    int value;

    if((value = regcomp(reegex, expr, REG_EXTENDED)) != 0) {
        printf("Error in regex\n");
        return -1;
    }

    if((value = regexec(reegex, tmp, nmatch, pmatch, REG_EXTENDED)) != 0) {
        printf("Failed to execute regex\n");
        return -1;
    }

    free(reegex);
    strcpy(tmp, tmp + 6);

    if(auth) {
        
        strcpy(tmp, tmp + 1);
        strcpy(aux, cutString(tmp, ':'));
        memcpy(components->user, aux, strlen(aux));

        strcpy(aux, cutString(tmp, '@'));
        memcpy(components->password, aux, strlen(aux));
        strcpy(tmp, tmp + 1);
    }

    strcpy(aux, cutString(tmp, '/'));
    memcpy(components->host, aux, strlen(aux));

    char* path = (char*) malloc(strlen(tmp));
    int start = 1;

    while(strchr(tmp, '/')) {
        aux = cutString(tmp, '/');

        if(start) {
            start = 0;
            strcpy(path, aux);
        } else {
            strcat(path, aux);
        }
        strcat(path, "/");
    }

    strcpy(components->path, path);
    strcpy(components->filename, tmp);

    free(tmp);
    free(aux);

    return 0;
}

char* cutString(char* str, char chr) {
    char* tmp = (char*) malloc(strlen(str));

    int idx = strlen(str) - strlen(strcpy(tmp, strchr(str, chr)));

    tmp[idx] = '\0';
    strncpy(tmp, str, idx);
    strcpy(str, str + strlen(tmp) + 1);

    return tmp;
}

/*

list_size([], 0).
list_size([H | T], S) :-    list_size(T, S1),
                            S is S1 + 1.

*/