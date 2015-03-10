CC = gcc
SRC = helpers.h helpers.c
C_FLAGS = -fPIC -shared

all: libhelpers.so

libhelpers.so:
	$(CC) -o libhelpers.so $(C_FLAGS) $(SRC)