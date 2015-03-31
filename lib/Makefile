CC = gcc
LIBDIR = .
CFLAG = -Wall -fPIC
LDFLAG = 

all: libhelpers.so libbufio.so

libhelpers.so: helpers.o
	$(CC) -o $@ $(LDFLAG) -shared $<

helpers.o:
	$(CC) -o $@ -c $(CFLAG) helpers.c

libbufio.so: bufio.o
	$(CC) -o $@ $(LDFLAG) -shared $<
	
bufio.o:
	$(CC) -o $@ -c $(CFLAG) bufio.c


clean:
	rm -rf *.o
	rm -rf *.so
