IDIR=./include
ODIR=./objects
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR) -Wall -g

LIBS=-lpthread

all: server client

$(ODIR)/%.o:$(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) 

server: $(ODIR)/server.o
	$(CC) -o server $< $(CFLAGS) $(LIBS)

client: $(ODIR)/client.o
	$(CC) -o client $< $(CFLAGS) $(LIBS)
	
clean:
	rm server client
	rm objects/*
