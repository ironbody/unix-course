#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h>

int main() {
  // create file descriptor
  int sfd = socket(AF_INET,SOCK_STREAM,0);

  // define server address and port
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(9001);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  bind(sfd, &serverAddr, sizeof(serverAddr));
  listen(sfd, 1);

  printf("Listening..\n");

  for(;;){

    int conn = accept(sfd, NULL, NULL);

    int pid = fork();

    if (pid == -1)
    {
      printf("Could not create new process\n");
      exit(-1);
    }
    
    if (pid == 0)
    {
      // child fork
      char msg[255] = "hejhejhej";
      send(conn, msg, sizeof(msg),0);

      exit(0);
    }

    signal(SIGCHLD,SIG_IGN);
  }

  printf("hello\n");  
}