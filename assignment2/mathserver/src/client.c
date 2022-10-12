#include <netinet/in.h> //structure for storing address information
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include "command.h"

int PORT = 9001;
in_addr_t IP = INADDR_ANY;

void Read_Options(int argc, char **argv);

int main(int argc, char **argv)
{

  Read_Options(argc, argv);

  int cd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT); // use some unused port number
  servAddr.sin_addr.s_addr = IP;

  printf("About to connect to the server\n");

  int connectStatus = connect(cd, (struct sockaddr *)&servAddr, sizeof(servAddr));
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

// Credit to Håkan Grahn for the following function
void Read_Options(int argc, char **argv)
{
  char *prog;

  prog = *argv;
  while (++argv, --argc > 0)
    if (**argv == '-')
      switch (*++*argv)
      {
      case 'h':
        printf("\nUsage: client [-p port] port to connect to  (default: 9001)\n");
        printf("           [-ip] IPv4 adress to connect to (default localhost)\n");
        printf("           [-h] help text\n");
        exit(0);
        break;
      case 'p':
        --argc;
        PORT = atoi(*++argv);
        break;
      case 'i':
        // checking for -ip without refactoring the function.
        if (*++*argv == 'p')
        {
          --argc;
          if (inet_pton(AF_INET, *++argv, &IP) != 1)
          {
            printf("Error: invalid IP adress\n");
            exit(0);
          }
          break;
        }
      default:
        printf("%s: ignored option: -%s\n", prog, *argv);
        printf("HELP: try %s -h \n\n", prog);
        break;
      }
}