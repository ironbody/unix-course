IDIR=./include
ODIR=./objects
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR) -Wall -g -std=gnu99

LIBS=-lpthread -lm

all: server client matinvpar kmeanspar

$(ODIR)/%.o:$(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

server: $(ODIR)/server.o
	$(CC) $(CFLAGS) -o server $< $(LIBS)

client: $(ODIR)/client.o
	$(CC) $(CFLAGS) -o client $< $(LIBS)
	
matinvpar: $(ODIR)/matrix_inverse_par.o
	$(CC) $(CFLAGS) -o matinvpar $< $(LIBS)

kmeanspar: $(ODIR)/kmeans_par.o
	$(CC) $(CFLAGS) -o kmeanspar $< $(LIBS)

clean:
	rm -f server client matinvpar kmeanspar
	rm -f objects/*
	rm -f kmeans-res*
	rm -f results.txt
	rm -f matrix_result.txt
