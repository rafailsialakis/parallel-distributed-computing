
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

double f(double a);

struct toSend{
	double local_sum;
};

struct toRecv{
    double  h;
    int     start;
    int     end;
};

int main(int argc, char *argv[]){
    int sockfd;
    struct sockaddr_in servaddr;
    struct toRecv toRecvPacket;
    struct toSend toSendPacket;

    double local_sum = 0;
    int start;
    int end;
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
    
    start = toRecvPacket.start;
    end = toRecvPacket.end;
    h = toRecvPacket.h;

    for(int i = start; i < end; i++){
        x = h * ( (double)i - 0.5);
        local_sum += f(x);
    }
    toSendPacket.local_sum = local_sum;
    printf("Local sum sent: %.10lf\n", local_sum);
    send(sockfd, &toSendPacket, sizeof(toSendPacket), 0);
}

double f(double a)
{
  return (4.0 / (1.0 + a*a));
}
