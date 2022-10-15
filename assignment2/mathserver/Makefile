IDIR=./include
ODIR=./objects
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR) -Wall -g -std=gnu99

LIBS=-lpthread

all: server client matinv kmeans

$(ODIR)/%.o:$(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

server: $(ODIR)/server.o
	$(CC) $(CFLAGS) $(LIBS) -o server $<

client: $(ODIR)/client.o
	$(CC) $(CFLAGS) $(LIBS) -o client $<
	
matinv: $(ODIR)/matrix_inverse_par.o
	$(CC) $(CFLAGS) $(LIBS) -o matinv $< -lm

kmeans: $(ODIR)/kmeans_par.o
	$(CC) $(CFLAGS) $(LIBS) -o kmeans $< -lm

clean:
	rm -f server client matinv kmeans
	rm -f objects/*
	rm -f kmeans-res*
	rm -f results.txt
