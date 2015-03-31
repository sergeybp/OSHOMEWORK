CC = gcc
LIBDIR = ../lib/
CFLAG = -I../lib/
LDFLAG = -L../lib/ -lhelpers

PROGRAM = bufcat

all: $(PROGRAM)

$(PROGRAM): ../lib/libbufio.so
	$(CC) $(PROGRAM).c $(CFLAG) $(LDFLAG) -o $(PROGRAM)

../lib/libbufio.so:
	$(MAKE) -C ../lib
	
clean:
	rm -rf bufcat
