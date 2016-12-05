NAME=interpret
CC=gcc
FLAGS=-std=c99 -Wall -Wextra -pedantic -Wunused -Wuninitialized -Winit-self -Wmaybe-uninitialized
CFILES=print_stack.c error.c memory_manager.c str.c ial.c keywords.c lex.c stack.c expr.c instrlist.c generator.c interpret.c parser.c
OFILES=print_stack.o error.o memory_manager.o str.o ial.o keywords.o lex.o stack.o expr.o instrlist.o generator.o interpret.o parser.o

TESTDIR=testy
MAKEDIR=$(shell pwd)

.PHONY: test

all:
	$(CC) $(FLAGS) $(CFILES) -c
	$(CC) $(FLAGS) -o $(NAME) $(OFILES)

test:
	bash $(TESTDIR)/test_script.sh $(MAKEDIR)/$(NAME)

clean:
	rm $(NAME) $(OFILES)
