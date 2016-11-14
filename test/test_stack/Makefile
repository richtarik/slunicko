# File: Makefile
# Riesenie: IFJ
# Datum: 05.10.2016
# Autor: Milos Molitoris
# Login: xmolit00
#....VUT FIT....#

CC=gcc
CFLAGS=-std=c99 -g -Wall -pedantic
VPATH = proj/test


all: test_stack

test_stack: test_stack.o stack.o 
	$(CC) $(CFLAGS) -o test_stack test_stack.o stack.o 
	
test_stack.o: test_stack.c ../../stack.h
	$(CC) $(CFLAGS) -c -o test_stack.o test_stack.c

stack.o: ../../stack.c ../../struct.h ../../stack.h
	$(CC) $(CFLAGS) -c -o stack.o ../../stack.c
	


	
clean:
	rm -f *.o test_stack