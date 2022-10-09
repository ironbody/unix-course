#if !defined(COMMAND_H)
#define COMMAND_H

// command the client sends to the server
// buf: string buffer where the input command is placed
struct command
{
    char buf[200];
};

// resulting filedata the client recieves
// size: size of the entire file
// buf: buffer where chunks of the file is placed
struct result {
    long size;
    char buf[1024];
};


#endif // COMMAND_H
