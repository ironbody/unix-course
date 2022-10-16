#include <netinet/in.h> //structure for storing address information
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
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

  for (;;)
  {
    printf("Please write your command: ");
    struct command cmd;
    fgets(cmd.buf, sizeof(cmd.buf), stdin);

    for (size_t i = 0; i < sizeof(cmd.buf); i++)
    {
      if (cmd.buf[i] == '\n')
      {
        cmd.buf[i] = '\0';
        break;
      }
    }

    send(cd, &cmd, sizeof(cmd), 0);

    struct file_data_result res;

    // recieving the size of the data
    int bytes = recv(cd, &res, sizeof(res), 0);
    if (bytes == 0)
    {
      printf("Connection was closed\n");
      exit(EXIT_SUCCESS);
    }
    else if (bytes == -1)
    {
      perror("Could not recieve file data");
      exit(EXIT_FAILURE);
    }
    

    res.size = ntohll(res.size);
    

    printf("Received a message from server! %lld bytes\n\n", res.size);

    printf("filename: %s\n", res.file_name);
    FILE* fp = fopen(res.file_name, "w");
    // FILE* fp = fopen("result.txt", "w");

    printf("Message: \n");

    char FILE_BUF[1024];

    long long recieved_data = 0;
    while (recieved_data < res.size)
    {
      int msize = recv(cd, &FILE_BUF, sizeof(FILE_BUF), 0);
      if (msize == 0)
      {
        printf("Connection closed\n");
        exit(EXIT_SUCCESS);
      }
      else if (msize == -1)
      {
          perror("Could not recieve file contents");
          exit(EXIT_FAILURE);
      }
      
      recieved_data += msize;
      // write(fp, res.buf, msize);
      fwrite(FILE_BUF, sizeof(char), msize, fp);

      printf("%s\n", FILE_BUF);
      printf("%lld\n", recieved_data);
    } 

    fclose(fp);
    // close(fp);
  }
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