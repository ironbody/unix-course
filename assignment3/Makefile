LDIR=./lib
SDIR=./src
BDIR=./bin
ODIR=./objects
CDIR=./lexyacc-code

CC=gcc

all: compiler libs

$(ODIR)/%.o:$(SDIR)/%.s
	gcc -g -c -o $@ $<

compiler: 
	cd $(CDIR); bison -y -d calc3.y 
	cd $(CDIR); flex calc3.l
	cd $(CDIR); gcc -c y.tab.c lex.yy.c
	cd $(CDIR); gcc y.tab.o lex.yy.o calc3a.c -o ../$(BDIR)/calc3a.exe
	cd $(CDIR); gcc y.tab.o lex.yy.o calc3b.c -o ../$(BDIR)/calc3b.exe
	cd $(CDIR); gcc y.tab.o lex.yy.o calc3g.c -o ../$(BDIR)/calc3g.exe
	cd $(CDIR); gcc y.tab.o lex.yy.o calc3i.c -o ../$(BDIR)/calc3i.exe

libs: fact gcd lntwo
	
fact: $(ODIR)/fact.o
	ar rcs $(LDIR)/libfact.a $(ODIR)/fact.o

gcd: $(ODIR)/gcd.o
	ar rcs $(LDIR)/libgcd.a $(ODIR)/gcd.o

lntwo: $(ODIR)/lntwo.o
	ar rcs $(LDIR)/liblntwo.a $(ODIR)/lntwo.o

clean:
	rm -f bin/*
	rm -f lib/*
	rm -f objects/*