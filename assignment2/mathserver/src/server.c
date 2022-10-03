#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

unsigned int PORT = 9001;
unsigned int DAEMON_FLAG = 0; //0 for false, 1 for true
enum strats{Fork, MuxBasic, MuxScale};
enum strats strat = Fork;

void Read_Options(int argc, char **argv);

int main(int argc, char **argv) {
  Read_Options(argc,argv);

  printf("port: %u\n", PORT);
  printf("daemon: %u\n", DAEMON_FLAG);
  printf("strat: %u\n", strat);
  
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
                printf("\nUsage: server [-p port] listen to port number (default: 9001)\n");
                printf("           [-d] daemon instead of normal program\n");
                printf("           [-s fork|muxbasic|muxscale] specify strategi (default: fork)\n");
                printf("           [-h] help text\n");
                exit(0);
                break;
            case 'd':
                DAEMON_FLAG = 1;
                break;
            case 'p':
                --argc;
                PORT = atoi(*++argv);
                break;
            case 's':
                --argc;
                char* in_strat = *++argv;
                printf("%s\n", in_strat);
                if (strcmp("fork", in_strat) == 0)
                {
                  strat = Fork;
                }
                else if (strcmp("muxbasic", in_strat) == 0)
                {
                  strat = MuxBasic;
                  // dont start multiple processes
                  // makes use of either select() or poll() system calls
                }
                else if (strcmp("muxscale", in_strat) == 0)
                {
                  strat = MuxScale;
                  // dont start multiple processes
                  // use of either epoll(), or kqueue() to multiplex between a number of file descriptors
                }
                else
                {
                  printf("Unknown strategy, defaulting to fork\n");
                  strat = Fork;
                }
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -h \n\n", prog);
                break;
            }
}