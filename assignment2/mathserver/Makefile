IDIR=./include
ODIR=./objects
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR) -Wall -g

LIBS=-lpthread

all: server client matinv kmeans

$(ODIR)/%.o:$(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) 

server: $(ODIR)/server.o
	$(CC) -o server $< $(CFLAGS) $(LIBS)

client: $(ODIR)/client.o
	$(CC) -o client $< $(CFLAGS) $(LIBS)
	
matinv: $(ODIR)/matrix_inverse_par.o
	$(CC) -o matinv $< $(CFLAGS) $(LIBS) 

kmeans: $(ODIR)/kmeans_par.o
	$(CC) -o kmeans $< $(CFLAGS) $(LIBS)

clean:
	rm server client matinv kmeans
	rm objects/*
