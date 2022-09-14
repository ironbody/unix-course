IDIR=./include
ODIR=./objects
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR) -Wall -g

LIBS=-lpthread

%.o: $(SDIR)/%.c
	$(CC) -c -o $(ODIR)/$@ $< $(CFLAGS) 

server: $(ODIR)/server.o
	$(CC) -o server $< $(CFLAGS) $(LIBS)