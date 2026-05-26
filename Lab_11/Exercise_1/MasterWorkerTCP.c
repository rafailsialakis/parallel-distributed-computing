#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 3000 
#define LISTENQ 5      /*maximum number waiting connections */

void *connection_handler(void *);

struct toRecv{
	double local_sum;
};

struct toSend{
    double  h;
    int     start;
    int     end;
};

struct WorkerArgs{
    int 	client_socket;
	int 	start;
	int 	end;
	double 	h;
};    

double sum = 0;
pthread_mutex_t mutex;
int main(int argc, char* argv[])
{
	if(argc != 3){
        fprintf(stderr, "Usage ./MasterWorker <Interval> <Number of workers>");
        return EXIT_FAILURE;
    }
    int n = strtol(argv[1], NULL, 10);
    int workers = strtol(argv[2], NULL, 10);
    int listenfd, connfd;
	int current_workers;
	double pi;
	double h = 1.0 / n;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct WorkerArgs *handler_args;
    pthread_t* threadIDs = malloc(sizeof(pthread_t) * workers);
    
    listenfd = socket (AF_INET, SOCK_STREAM, 0);
  
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    
	int block = n / workers;
	//printf("Block = %d\n", block);

    for(current_workers = 0; current_workers < workers; current_workers++){
		printf("Server running...waiting for connections.\n");
		clilen = sizeof(cliaddr);
		connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
		printf("Connection accepted ... \n");

		handler_args = (struct WorkerArgs*) malloc(sizeof(struct WorkerArgs)); 
		
		handler_args->start = current_workers * block;
		handler_args->end = (current_workers + 1) * block;
		handler_args->h = h;
		handler_args -> client_socket = connfd;
		
		pthread_create(&threadIDs[current_workers], NULL,  connection_handler, (void *)handler_args);
		printf("Handler thread assigned\n");
    }
	for(int i = 0; i < workers; i++){
		pthread_join(threadIDs[i], NULL);
	}
    close(listenfd);
	free(threadIDs);

	pi = h * sum;
	printf("Value of PI is %.15lf\n", pi);
    return EXIT_SUCCESS;
}


void *connection_handler(void *args) {
	int socket; 
	int read_len;
	int send_status;

	struct toRecv toRecvPacket;
	struct toSend toSendPacket;
	struct WorkerArgs* arguments = (struct WorkerArgs*) args;
	
	socket = arguments->client_socket;
	toSendPacket.start = arguments->start;
	toSendPacket.end = arguments->end;
	toSendPacket.h = arguments->h;

	//detach and self destroy on termination
	//pthread_detach(pthread_self());
	free (args);
	//DEBUG
	fprintf(stdout, "Assigned block from %d to %d to thread %ld\n", toSendPacket.start, toSendPacket.end, pthread_self());

	send(socket, &toSendPacket, sizeof(toSendPacket), 0);	
	read_len = recv(socket, &toRecvPacket, sizeof(toRecvPacket), 0);

	pthread_mutex_lock(&mutex);
	sum += toRecvPacket.local_sum;
	printf("Sum %.10lf\n", sum); 
	pthread_mutex_unlock(&mutex);

	printf ("Connection closed, partial calculation finished...\n"); 
	return 0;
}
 