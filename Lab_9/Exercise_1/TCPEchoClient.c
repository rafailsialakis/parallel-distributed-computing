
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define EXITMESSAGE "EXIT"

int
main(int argc, char *argv[])
{
 int sockfd;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];

 if (argc !=3) {
  printf("Usage: TCPClient <IP address> <port number>\n");
  exit(1);
 }

 sockfd = socket (AF_INET, SOCK_STREAM, 0);

 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(strtol(argv[2], NULL, 10)); //convert to big-endian order

 connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)); 
 
 while (1) {
  printf ("Enter a message to send\n");
  fgets(sendline, MAXLINE, stdin);

  send(sockfd, sendline, strlen(sendline), 0);
  
  sendline[strlen(sendline)-1] = '\0'; /*remove \n and put \0 instead */
  if  (strcmp(sendline, EXITMESSAGE ) == 0) {break;}
  
  recv(sockfd, recvline, MAXLINE,0);
  printf("String received from the server: ");
  fputs(recvline, stdout);
  printf("\n");
  
 }

 exit(0);
}