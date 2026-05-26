#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3002 /*port*/
#define LISTENQ 5      /*maximum number waiting connections */
#define EXITMESSAGE "EXIT"
#define MAX_CLIENTS 100

void process_message(char *msg_in, char *msg_out);
void *connection_handler(void *);
void broadcast(int* sockets, char* sendmsg, int client_count, int socket);

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_args {
    int client_socket;
};    

int main()
{
    int listenfd, connfd;
    socklen_t clilen;
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
	pthread_mutex_lock(&client_mutex);
	if (client_count < MAX_CLIENTS) {
		client_sockets[client_count++] = connfd;
	} else {
		printf("Too many clients. Connection rejected.\n");
		close(connfd);
		pthread_mutex_unlock(&client_mutex);
		continue;
	}
	pthread_mutex_unlock(&client_mutex);
	printf("Connection accepted ... \n");

	handler_args = (struct thread_args *) malloc(sizeof(struct thread_args)); 
	handler_args -> client_socket = connfd;
	
	pthread_create( &threadID, NULL,  connection_handler, (void *)handler_args);
    	printf("Handler thread assigned\n");
    }

    close(listenfd);
    return 0;
}


void *connection_handler(void *args) {
	int socket; 
	int read_len;
	int send_status;
	char send_msg[MAXLINE], recv_msg[MAXLINE];
	struct thread_args* arguments = (struct thread_args*) args;
	//detach and self destroy on termination
	pthread_detach(pthread_self());
	socket = arguments -> client_socket;
	//free (args);
	
	while (1)
	{
		//does not work for empty string
		read_len = recv(socket, recv_msg, MAXLINE, 0);
		
		recv_msg[read_len-1] = '\0'; /*remove \n and put \0 instead */
		if  (strcmp(recv_msg, EXITMESSAGE ) == 0) {break;}
        //strcat(msg_in, "\n"); /*put \n back */		
		//printf("received %s %s\n", recv_msg, EXITMESSAGE);
		process_message (recv_msg, send_msg);
	
		//printf("send %s\n", send_msg);
		broadcast(client_sockets, send_msg, client_count, arguments->client_socket);	
	}
	printf ("Connection closed, handler thread exiting ...\n"); 
	return 0;
	
}

void process_message(char *msg_in, char *msg_out) {
    strcpy (msg_out, msg_in);
}  

void broadcast(int* sockets, char* sendmsg, int client_count, int socket){
	for(int i = 0; i < client_count; i++){
		if(sockets[i] != socket){
			send(sockets[i], sendmsg, strlen(sendmsg), 0);
		}
	}
}