IDIR=./include
ODIR=./objects
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR) -Wall -g -std=gnu99

LIBS=-lpthread

all: server client matinvpar kmeanspar

$(ODIR)/%.o:$(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

server: $(ODIR)/server.o
	$(CC) $(CFLAGS) $(LIBS) -o server $<

client: $(ODIR)/client.o
	$(CC) $(CFLAGS) $(LIBS) -o client $<
	
matinvpar: $(ODIR)/matrix_inverse_par.o
	$(CC) $(CFLAGS) $(LIBS) -o matinvpar $< -lm

kmeanspar: $(ODIR)/kmeans_par.o
	$(CC) $(CFLAGS) $(LIBS) -o kmeanspar $< -lm

clean:
	rm -f server client matinvpar kmeanspar
	rm -f objects/*
	rm -f kmeans-res*
	rm -f results.txt
	rm -f matrix_result.txt
