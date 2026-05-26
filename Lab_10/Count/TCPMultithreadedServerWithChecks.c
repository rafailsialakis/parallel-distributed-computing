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
pthread_mutex_t RWMUTEX;

struct list{
    int num;
    double pi;
    struct list* next;
};

void process_message(char *msg_in, char *msg_out);
void *connection_handler(void *);
double f(double);
double check_list(struct list* head, int num);
void write_to_list(struct list** head, int num, double pi);
double calc_pi(int num);

struct thread_args {
    int client_socket;
    struct list** l;
};    

int main()
{
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct thread_args *handler_args;
    pthread_t threadID;
    struct list* head = NULL;
    
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
        handler_args->l = &head;

        pthread_mutex_init(&RWMUTEX, NULL);
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
    struct thread_args* arguments = (struct thread_args*)args;
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
		
		int num = strtol(recv_msg, NULL, 10);

        double pi;
        double result = check_list(*arguments->l, num);
        if(result){
            pi = result;
        }else{
            pi = calc_pi(num);
            write_to_list(arguments->l, num, pi);
        }
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

double check_list(struct list* head, int num){
    pthread_mutex_lock(&RWMUTEX);
    struct list* current = head;
    while(current != NULL){
        if(current->num == num){
            printf("Found element with num = %d and pi = %.10lf\n", current->num, current->pi);
            pthread_mutex_unlock(&RWMUTEX);
            return current->pi;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&RWMUTEX);
    return 0; 
}

void write_to_list(struct list** head, int num, double pi) {
    pthread_mutex_lock(&RWMUTEX);
    struct list* new_node = malloc(sizeof(struct list));
    new_node->num = num;
    new_node->pi = pi;
    new_node->next = NULL;

    //DEBUG
    printf("New node in list with num = %d and pi = %.10lf\n", num,pi);
    if (*head == NULL) {
        *head = new_node;
        pthread_mutex_unlock(&RWMUTEX);
        return;
    }
    struct list* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    pthread_mutex_unlock(&RWMUTEX);
}

double calc_pi(int num){
    double h = 1.0 / (double) num ;
    double sum = 0.0;
    double pi, x;
	for (long int i = 1; i <= num; i++) {
		x = h * ((double)i - 0.5);
        sum += f(x);
	}
	pi = h * sum;
    return pi;
}

