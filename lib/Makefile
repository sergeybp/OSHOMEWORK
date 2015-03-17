CC = gcc
LIBDIR = .
CFLAG = -Wall -fPIC
LDFLAG = 

all: libhelpers.so

libhelpers.so: helpers.o
	$(CC) -o $@ $(LDFLAG) -shared $<

helpers.o:
	$(CC) -o $@ -c $(CFLAG) helpers.c
	
clean:
	rm -rf *.o
	rm -rf *.so
