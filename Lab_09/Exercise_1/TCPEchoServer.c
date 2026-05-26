#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3002 /*port*/
#define LISTENQ 5 /*maximum number of waiting client connections */
#define EXITMESSAGE "EXIT"

void to_lower(char* buf);
void to_upper(char* buf);
void encode_cipher(char* buf, int key);
void decode_cipher(char* buf, int key);

int main (int argc, char **argv){
    int listenfd, connfd, n;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;


    listenfd = socket (AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    printf("Server running...waiting for connections.\n");

    clilen = sizeof(cliaddr);
    connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
    printf("Connection accepted..\n");

    while (1)
    {
        //does not work for empty string
        n = recv(connfd, buf, MAXLINE, 0);

        if (n <= 0) break;
        buf[n] = '\0'; 
        if  (strcmp(buf, EXITMESSAGE ) == 0) {break;}
        char* choice_str = strtok(buf, "#"); 
        char* input = strtok(NULL, "#");
        char* key_str = strtok(NULL, "#");
        if(!choice_str || !input || !key_str){
            strcpy(buf, "Invalid format. Use <type#input#key>");
            send(connfd, buf, strlen(buf), 0);
            continue;
        }
        int choice = strtol(choice_str, NULL, 10);
        int key = strtol(key_str, NULL, 10);
        switch (choice)
        {
        case 1:
            to_lower(input);
            break;
        case 2:
            to_upper(input);
            break;
        case 3:
            encode_cipher(input, key);
            break;
        case 4:
            decode_cipher(input, key);
            break;
        default:
            strcpy(buf, "Invalid type. Use 1-4.");
            send(connfd, buf, strlen(buf), 0);
            continue;
        }
        //process request here
        send(connfd, input, strlen(input),0);	
    }   

    close(connfd);
    close (listenfd);
}

void to_lower(char* buf){
    for(int i = 0; buf[i]; i++){
        if(buf[i] >= 'A' && buf[i] <= 'Z'){
            buf[i]+=32;
        }
    }
}
void to_upper(char* buf){
    for(int i = 0; buf[i]; i++){
        if(buf[i] >= 'a' && buf[i] <= 'z'){
            buf[i]-=32;
        }
    }
}
void encode_cipher(char* buf, int key){
    for (int i = 0; buf[i]; i++) {
        if (buf[i] >= 'A' && buf[i] <= 'Z') {
            buf[i] = 'A' + (buf[i] - 'A' + key) % 26;
        }else if(buf[i]>='a' && buf[i] <= 'z') {
            buf[i] = 'a' + (buf[i] - 'a' + key) % 26;
        }
    }
}

void decode_cipher(char* buf, int key){
    for (int i = 0; buf[i]; i++) {
        if (buf[i] >= 'A' && buf[i] <= 'Z') {
            buf[i] = 'A' + (26 + buf[i] - 'A' - key) % 26;
        }else if(buf[i]>='a' && buf[i] <= 'z') {
            buf[i] = 'a' + (26 + buf[i] - 'a' - key) % 26;
        }
    }
}