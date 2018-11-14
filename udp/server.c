#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>     /* defines STDIN_FILENO, system calls,etc */
#include <sys/types.h>  /* system data type definitions */
#include <sys/socket.h> /* socket specific definitions */
#include <netinet/in.h> /* INET constants and stuff */
#include <arpa/inet.h>  /* IP address conversion stuff */
#include <netdb.h>      /* gethostbyname */


#define MAXBUF 1024*1024

void echo(int sd) {
    char bufin[MAXBUF];
    struct sockaddr_in remote;
    int num = 1;
    char int2char[MAXBUF];

    socklen_t len = sizeof(remote);
    while (1) {
      if(num > 50 )
        break;

      int n = recvfrom(sd, bufin, MAXBUF, 0, (struct sockaddr *) &remote, &len);
      sprintf(int2char,"%d",num);
      if (n < 0) {
        perror("Error receiving data");
      } else {
        strcat(int2char, " ");
        strcat(int2char, bufin);
        sendto(sd, int2char, n + 3, 0, (struct sockaddr *)&remote, len);
      }
      num++;
    }
}

int main() {
  int ld;
  struct sockaddr_in skaddr;
  struct sockaddr_in client_socket_addr;
  socklen_t length;
  int server_socket;

  if ((server_socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Problem creating socket\n");
    exit(1);
  }

  skaddr.sin_family = AF_INET;
  skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  skaddr.sin_port = htons(9876);

  if (bind(server_socket, (struct sockaddr *) &skaddr, sizeof(skaddr)) < 0) {
    perror("Problem binding\n");
    exit(0);
  }
 
  length = sizeof(skaddr);
  if (getsockname(server_socket, (struct sockaddr *) &skaddr, &length) < 0) {
    perror("Error getsockname\n");
    exit(1);
  }
  printf("address = %s:%d\n",inet_ntoa(skaddr.sin_addr),ntohs(skaddr.sin_port));

  echo(server_socket);
  return 0;
}
