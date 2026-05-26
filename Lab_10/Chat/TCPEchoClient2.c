#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096
#define SERV_PORT 3002
#define EXITMESSAGE "EXIT"

int sockfd;

void *receive_handler(void *arg) {
    char recv_msg[MAXLINE];
    int n;

    while ((n = recv(sockfd, recv_msg, MAXLINE - 1, 0)) > 0) {
        recv_msg[n] = '\0';
        printf("Broadcast: %s\n",sockfd, recv_msg);
    }

    printf("Disconnected from server.\n");
    pthread_exit(NULL);
}

int main() {
    struct sockaddr_in servaddr;
    char send_msg[MAXLINE];
    pthread_t recv_thread;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connect failed");
        exit(1);
    }

    printf("Connected to server. Type messages, or 'EXIT' to quit.\n");

    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    while (fgets(send_msg, MAXLINE, stdin) != NULL) {
        send_msg[strcspn(send_msg, "\n")] = '\0'; 
        send(sockfd, send_msg, strlen(send_msg) + 1, 0);

        if (strcmp(send_msg, EXITMESSAGE) == 0) {
            break;
        }
    }

    close(sockfd);
    pthread_cancel(recv_thread); 
    pthread_join(recv_thread, NULL);
    return 0;
}

