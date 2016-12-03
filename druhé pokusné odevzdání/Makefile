NAME=interpret
CC=gcc
FLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -Wunused -Wuninitialized -Winit-self -Wmaybe-uninitialized
CFILES=error.c memory_manager.c str.c ial.c keywords.c lex.c stack.c expr.c instrList.c generator.c interpret.c parser.c
OFILES=error.o memory_manager.o str.o ial.o keywords.o lex.o stack.o expr.o instrList.o generator.o interpret.o parser.o

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
