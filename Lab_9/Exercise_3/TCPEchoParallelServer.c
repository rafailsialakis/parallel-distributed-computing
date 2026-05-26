#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 5 /*maximum number of waiting client connections */
#define EXITMESSAGE "EXIT"

struct thread_args {
    int client_socket;
};
void* connection_handler(void* args);
void to_lower(char* buf);
void to_upper(char* buf);
void encode_cipher(char* buf, int key);
void decode_cipher(char* buf, int key);

int main (int argc, char **argv){
    int listenfd, connfd, n;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
    struct thread_args *handler_args;
    pthread_t threadID;

    listenfd = socket (AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    while (1) {
    
        printf("Server running...waiting for connections.\n");
        clilen = sizeof(cliaddr);
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("Connection accepted ... \n");
    
        handler_args = (struct thread_args *) malloc(sizeof(struct thread_args)); 
        handler_args -> client_socket = connfd;
        
        pthread_create( &threadID, NULL,  connection_handler, (void *)handler_args);
        printf("Handler thread assigned\n");
    }
    close(connfd);
    close (listenfd);
}
void* connection_handler(void* args){
	int socket; 
	int read_len;
	int send_status;
	char send_msg[MAXLINE], recv_msg[MAXLINE];
	
	//detach and self destroy on termination
	pthread_detach(pthread_self());
	socket = ((struct thread_args*)args) -> client_socket;
	free (args);
    while (1)
    {
        //does not work for empty string
        read_len = recv(socket, recv_msg, MAXLINE, 0);

        if (read_len <= 0) break;
        recv_msg[read_len] = '\0'; 
        if  (strcmp(recv_msg, EXITMESSAGE ) == 0) {break;}
        char* choice_str = strtok(recv_msg, "#"); 
        char* input = strtok(NULL, "#");
        char* key_str = strtok(NULL, "#");
        if(!choice_str || !input || !key_str){
            strcpy(recv_msg, "Invalid format. Use <type#input#key>");
            send(socket, recv_msg, strlen(recv_msg), 0);
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
            strcpy(recv_msg, "Invalid type. Use 1-4.");
            send(socket, recv_msg, strlen(recv_msg), 0);
            continue;
        }
        //process request here
        send(socket, input, strlen(input),0);	
    }   
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