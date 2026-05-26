
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

double f(double a);
void* work(void*);
struct toSend{
	double local_sum;
};

struct toRecv{
    double  h;
    int     start;
    int     end;
};

struct threadArgs{
    double h;
    int from;
    int to;
    double local_sum;
};

int main(int argc, char *argv[]){
    int sockfd;
    struct sockaddr_in servaddr;
    struct toRecv toRecvPacket;
    struct toSend toSendPacket;

    long int NUM_OF_PROCS = sysconf(_SC_NPROCESSORS_ONLN);

    pthread_t threadIDs[NUM_OF_PROCS];
    struct threadArgs args[NUM_OF_PROCS];
    
    double partial_sum = 0;
    int block;

    double h;
    double x;

    if (argc !=3) {
        printf("Usage: TCPClient <IP address> <port number>\n");
        return EXIT_FAILURE;
    }

    sockfd = socket (AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]);
    servaddr.sin_port =  htons(strtol(argv[2], NULL, 10)); //convert to big-endian order

    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); 

    recv(sockfd, &toRecvPacket, sizeof(toRecvPacket) ,0);
    block = (toRecvPacket.end - toRecvPacket.start) / NUM_OF_PROCS;
    for(int i = 0; i < NUM_OF_PROCS; i++){
        args[i].from = toRecvPacket.start + (i * block);
        args[i].to = toRecvPacket.start + (i+1) * block;
        args[i].h = toRecvPacket.h;
        args[i].local_sum = 0;
        pthread_create(&threadIDs[i], NULL, work, &args[i]);
    }
    for(int i = 0; i < NUM_OF_PROCS; i++){
        pthread_join(threadIDs[i], NULL);
    }
    for(int i = 0; i < NUM_OF_PROCS; i++){
        partial_sum += args[i].local_sum;
    }
    toSendPacket.local_sum = partial_sum;
    //printf("Local sum sent: %.10lf\n", partial_sum); DEBUG
    send(sockfd, &toSendPacket, sizeof(toSendPacket), 0);
}

void* work(void* args){
    struct threadArgs* arguments = (struct threadArgs*) args;
    int from = arguments->from;
    int to = arguments->to;
    double h = arguments->h;
    double local_sum = arguments->local_sum;
    double x;
    for(int i = arguments->from; i < arguments->to; i++){
        x = h * ( (double)i - 0.5);
        local_sum += f(x);
    }
    arguments->local_sum = local_sum;
}

double f(double a)
{
  return (4.0 / (1.0 + a*a));
}
