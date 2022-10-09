#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include "command.h"

int main()
{

  int cd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(9001); // use some unused port number
  servAddr.sin_addr.s_addr = INADDR_ANY;

  printf("About to connect to the server\n");

  int connectStatus = connect(cd, &servAddr, sizeof(servAddr));
  if (connectStatus == -1)
  {
    printf("Error: cannot connect to server\n");
    exit(0);
  }

  printf("Established connection with the server!\n\n");

  printf("Please write your command: ");
  struct command cmd;
  gets(cmd.buf);

  send(cd, &cmd, sizeof(cmd), 0);

  struct result res;

  // recieving the size of the data
  recv(cd, &res.size, sizeof(res.size), 0);

  printf("Received a message from server! %ld bytes\n\n", res.size);

  FILE *fp = fopen("results.txt", "w");

  printf("Message: \n");
  long recieved_data = 0;
  do
  {
    int msize = recv(cd, &res.buf, sizeof(res.buf), 0);
    recieved_data += msize;
    printf("%s\n", res.buf);
    printf("%ld\n", recieved_data);
    fprintf(fp, "%s", res.buf);
  } while (recieved_data < res.size);

  printf("\n");

  // printf("Hello\n");
}