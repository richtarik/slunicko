# File: Makefile
# Riesenie: IFJ
# Datum: 05.10.2016
# Autor: Milos Molitoris
# Login: xmolit00
#....VUT FIT....#

CC=gcc
CFLAGS=-std=c99 -g -Wall -pedantic
VPATH = proj/test


all: test_lex

test_lex: test_lex.o lex.o keywords.o
	$(CC) $(CFLAGS) -o test_lex test_lex.o lex.o keywords.o
	
test_lex.o: test_lex.c ../../lex.h
	$(CC) $(CFLAGS) -c -o test_lex.o test_lex.c

keywords.o: ../../keywords.c ../../keywords.h ../../token.h
	$(CC) $(CFLAGS) -c -o keywords.o ../../keywords.c

lex.o: ../../lex.c ../../token.h ../../lex.h
	$(CC) $(CFLAGS) -c -o lex.o ../../lex.c
	


	
clean:
	rm -f *.o test_lex