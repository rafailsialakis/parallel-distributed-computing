#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 5 /*maximum number of waiting client connections */
#define EXITMESSAGE "EXIT"

void operation(char* input, int num1, char op, int num2);
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
        int num1, num2;
        char op;
        char input[32];
        memset(input,0,sizeof(input));

        if (sscanf(buf, "%d%c%d", &num1, &op, &num2) == 3) {
            printf("Parsed: num1 = %d, op = '%c', num2 = %d\n", num1, op, num2);
            operation(input, num1, op, num2);
        } else {
            strcpy(buf,"Invalid format. Use <num><operator><num>.\n");
            send(connfd, buf, strlen(buf), 0);
            continue;
        }
        //debugging
        printf("String received from and resent to the client:\n");
        puts(buf);

        //process request here
        send(connfd, input, strlen(input),0);	
    }   

    close(connfd);
    close (listenfd);
}

void operation(char* result, int num1, char op, int num2){
    switch (op) {
        case '+':
            sprintf(result, "%d", num1 + num2);
            break;
        case '-':
            sprintf(result, "%d", num1 - num2);
            break;
        case '*':
            sprintf(result, "%d", num1 * num2);
            break;
        case '/':
            if (num2 == 0) {
                strcpy(result, "Error: Division by zero");
            } else {
                sprintf(result, "%d", num1 / num2);
            }
            break;
        default:
            strcpy(result, "Invalid operator. Use +, -, *, /");
    }
}