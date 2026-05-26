#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3001	 /*port*/
#define LISTENQ 5      /*maximum number waiting connections */
#define EXITMESSAGE "EXIT"

void process_message(char *msg_in, char *msg_out);
void *connection_handler(void *);
double f(double);

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
	
	//detach and self destroy on termination
	pthread_detach(pthread_self());
	socket = ((struct thread_args*)args) -> client_socket;
	free (args);
	
	while (1)
	{
		//does not work for empty string
		read_len = recv(socket, recv_msg, MAXLINE, 0);
		
		recv_msg[read_len-1] = '\0'; /*remove \n and put \0 instead */
		if  (strcmp(recv_msg, EXITMESSAGE ) == 0) {break;}
		
		int num = strtol(recv_msg, NULL, 10);
		double PI25DT = 3.141592653589793238462643;
		
		double h = 1.0 / (double) num ;
		double sum = 0.0;
		double pi, x;

		for (long int i = 1; i <= num; i++) {
			x = h * ((double)i - 0.5);
			sum += f(x);
		}

		pi = h * sum;

   		sprintf(send_msg, "pi is approximately %.16f\n", pi);

		//printf("send %s\n", send_msg);
		send(socket, send_msg, strlen(send_msg),0);	
	}
	printf ("Connection closed, handler thread exiting ...\n"); 
	return 0;	
}

double f(double x) {
    return 4.0 / (1.0 + x * x);
}
