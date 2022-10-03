#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>

int main()
{

  int cd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(9001); // use some unused port number
  servAddr.sin_addr.s_addr = INADDR_ANY;

  printf("About to connect to the server\n");

  int connectStatus = connect(cd, &servAddr, sizeof(servAddr));

  if (connectStatus != -1)
  {
    printf("Established connection with the server!\n\n");
    char strData[255];
    printf("Press any key\n");
    getchar();

    int msize = recv(cd, strData, sizeof(strData), 0);
    printf("Received a message from server! %d bytes\n\n", msize);
    printf("Message: %s\n", strData);
  }

  printf("Hello\n");
}