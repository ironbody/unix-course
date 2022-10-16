#define _GNU_SOURCE
#include <netinet/in.h> //structure for storing address information
#include <signal.h>     // SIGCHLD and SIG_IGN
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
// #include <sys/sendfile.h>
#include "command.h"

unsigned int PORT = 9001;
unsigned int DAEMON_FLAG = 0; // 0 for false, 1 for true
enum strats
{
  Fork,
  MuxBasic,
  MuxScale
};
enum strats strat = Fork;

void Read_Options(int argc, char **argv);
void daemonize(const char *cmd);
void handle_conn(int sock, unsigned long long id);
void extract_args(char *args_str, int res_size, char **res);
int count_spaces(char *args_str);
void create_results_dir();
void send_nothing(int sock);

int main(int argc, char **argv)
{
  signal(SIGCHLD, SIG_IGN);

  Read_Options(argc, argv);

  create_results_dir();

  if (DAEMON_FLAG == 1)
  {
    daemonize("me");
  }

  // printf("port: %u\n", PORT);
  // printf("daemon: %u\n", DAEMON_FLAG);
  // printf("strat: %u\n", strat);

  // create file descriptor
  int sfd = socket(AF_INET, SOCK_STREAM, 0);

  // define server address and port
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  bind(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  listen(sfd, 1);

  printf("Listening..\n");

  for (unsigned long long i = 0;; i++)
  {

    int conn = accept(sfd, NULL, NULL);

    int pid = fork();

    if (pid == -1)
    {
      printf("Could not create new process\n");
      exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
      handle_conn(conn, i);
    }
  }

  // printf("hello\n");
}

void handle_conn(int sock, unsigned long long id)
{

  printf("Handled connection #%llu\n", id);

  for (;;)
  {
    struct command cmd;

    int bytes_rec = recv(sock, &cmd, sizeof(cmd), 0);
    if (bytes_rec == -1)
    {
      perror("could not recieve data");
      exit(EXIT_FAILURE);
    }
    
    if (bytes_rec == 0)
    {
      printf("Connection closed!\n");
      break;
    }

    printf("Recieved: %s\n", cmd.buf);

    // num of args is num of spaces +1,
    // +3 because inserting "-o filename" is +2 args,
    // and space for a NULL arg at the end is +1
    int in_arg_count = count_spaces(cmd.buf) + 1;
    printf("In_arg_count: %d\n", in_arg_count);
    int total_arg_count = in_arg_count + 3;
    char *args[total_arg_count];
    extract_args(cmd.buf, in_arg_count, args);

    char *out_file = NULL;
    asprintf(&out_file, "./computed_results/client-%llu.txt", id);
    args[total_arg_count - 3] = "-o";
    args[total_arg_count - 2] = out_file;
    args[total_arg_count - 1] = NULL;

    for (size_t i = 0; i < total_arg_count - 1; i++)
    {
      printf("%s\n", args[i]);
    }

    char *path;
    if (strcmp(args[0], "matinvpar") == 0)
    {
      path = "./matinvpar";
    }
    else if (strcmp(args[0], "kmeanspar") == 0)
    {
      path = "./kmeanspar";
    }
    else
    {
      printf("bad input\n");
      send_nothing(sock);
      continue; // loop again
    }

    pid_t pid = fork();
    if (pid == -1)
    {
      perror("Could not create new process");
      send_nothing(sock);
      close(sock);
      exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
      execv(path, args);
    }

    int status;
    waitpid(pid, &status, 0);
    printf("After waitpid\n");

    FILE *fd = fopen(out_file, "rwx");
    if (fd == NULL)
    {
      perror("Could not open result file");
      send_nothing(sock);
      close(sock);
      fclose(fd);
      exit(EXIT_FAILURE);
    }

    struct stat file_stat;
    if (fstat(fileno(fd), &file_stat) < 0)
    {
      perror("Could not get file stats");
      send_nothing(sock);
      close(sock);
      fclose(fd);
      exit(EXIT_FAILURE);
    }

    struct result res;

    res.size = file_stat.st_size;
    printf("File size: %lld\n", file_stat.st_size);
    // send filesize
    send(sock, &res.size, sizeof(file_stat.st_size), 0);

    // credit to brian campbell on stackoverflow for the following loop
    // https://stackoverflow.com/a/2014066
    for (;;)
    {
      int bytes_read = read(fileno(fd), &res.buf, sizeof(res.buf));
      // printf("bytes_read: %d\n", bytes_read);
      if (bytes_read == 0)
      {
        break;
      }
      if (bytes_read < 0)
      {
        send(sock, "\0", sizeof("\0"), 0);
        close(sock);
        fclose(fd);
        exit(EXIT_FAILURE);
      }

      // we use a loop because all of the data may not be written in a single write call.
      char *p = res.buf;
      while (bytes_read > 0)
      {
        // write() is equivalent to send() with a flag of 0
        int bytes_written = write(sock, p, bytes_read);
        if (bytes_written <= 0)
        {
          perror("unable to send data");
          close(sock);
          fclose(fd);
          exit(EXIT_FAILURE);
        }
        // printf("bytes_written: %d\n", bytes_written);

        bytes_read -= bytes_written;
        p += bytes_written;
      }
    }

    free(out_file);
    fclose(fd);
  }
  
  close(sock);
  exit(EXIT_SUCCESS);
}

int count_spaces(char *args_str)
{
  int space_nr = 0;
  for (int i = 0; i < strlen(args_str); i++)
  {
    if (args_str[i] == ' ')
    {
      space_nr += 1;
    }
  }
  return space_nr;
}

void extract_args(char *args_str, int res_size, char **res)
{
  res[0] = strtok(args_str, " ");
  for (int i = 1; i < res_size; i++)
  {
    res[i] = strtok(NULL, " ");
  }

  // for (int i = 0; i < (res_size+1); i++)
  //     printf("%s\n", args[i]);
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
        exit(EXIT_SUCCESS);
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
        char *in_strat = *++argv;
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

// code provided in lecture 10 in our course
void daemonize(const char *cmd)
{
  // int i, fd0, fd1, fd2;
  int i;
  pid_t pid;
  struct rlimit rl;
  struct sigaction sa;

  /* STEP 1: Clear file creation mask */
  umask(0);

  /* Get maximum number of file descriptors */
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
  {
    perror(cmd);
    exit(1);
  }

  /* STEP 2a: Fork a child process */
  if ((pid = fork()) < 0)
  {
    perror(cmd);
    exit(1);
  }
  else if (pid != 0)
  { /* STEP 2b: Exit the parent process */
    exit(0);
  }
  /* STEP 3: Become a session leader to lose controlling TTY
   * The child process executes this! */
  setsid();

  /* Ensure future opens won't allocate controlling TTYs */
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0)
  {
    perror("Can't ignore SIGHUP");
    exit(1);
  }
  if ((pid = fork()) < 0)
  {
    perror("Can't fork");
    exit(1);
  }
  else if (pid != 0) /* parent */
    exit(0);

  /*
   *      * Change the current working directory to the root so
   *           * we won't prevent file systems from being unmounted.
   *                */
  if (chdir("/") < 0)
  {
    perror("Can't change to /");
    exit(1);
  }

  /* Close all open file descriptors */
  printf("limit: %llu\n", rl.rlim_max);
  if (rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = 1024;
  for (i = 0; i < rl.rlim_max; i++)
    close(i);

  /* Attach file descriptors 0, 1, and 2 to /dev/null */
  // fd0 = open("/dev/null", O_RDWR);
  // fd1 = dup(0);
  // fd2 = dup(0);
}

void create_results_dir()
{
  struct stat st = {0};
  if (stat("./computed_results", &st) == -1) // if the directory does not exist
  {
    if (mkdir("./computed_results", 0777) != 0) // if the directory could not be created
    {
      perror("Could not create results directory\n");
    }
  }
}

// sends nothing to the client
void send_nothing(int sock)
{
  char msg[] = "";
  long size = (long)sizeof(msg);
  send(sock, &size, sizeof(size), 0);
  send(sock, &msg, sizeof(msg), 0);
}
