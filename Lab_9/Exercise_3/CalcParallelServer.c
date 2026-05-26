#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3001 /*port*/
#define LISTENQ 5 /*maximum number of waiting client connections */
#define EXITMESSAGE "EXIT"

struct thread_args {
    int client_socket;
}; 

void* connection_handler(void* args);
void process_message(char *msg_in, char *msg_out);
void operation(char* input, int num1, char op, int num2);
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
    socket = ((struct thread_args*)args)->client_socket;
	free (args);

    while (1)
    {
        //does not work for empty string
        read_len = recv(socket, recv_msg, MAXLINE, 0);
        if (read_len <= 0) break;
        recv_msg[read_len] = '\0'; 
        if  (strcmp(recv_msg, EXITMESSAGE ) == 0) {break;}
        int num1, num2;
        char op;
        char input[32];
        memset(input,0,sizeof(input));
        if (sscanf(recv_msg, "%d%c%d", &num1, &op, &num2) == 3) {
            printf("Parsed: num1 = %d, op = '%c', num2 = %d\n", num1, op, num2);
            operation(send_msg, num1, op, num2);
        } else {
            strcpy(recv_msg,"Invalid format. Use <num><operator><num>.\n");
            send(socket, recv_msg, strlen(recv_msg), 0);
            continue;
        }
        //debugging
        printf("String received from and resent to the client:\n");
        puts(recv_msg);
        //process request here
        send(socket, send_msg, strlen(send_msg),0);	
    }   
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

void process_message(char *msg_in, char *msg_out) {

    //strcat(msg_in, "\n"); /*put \n back */
    strcpy (msg_out, msg_in);
 
}  