FLAGS= -std=c99 -O0 -Wall -Wextra -Werror -pedantic -Wuninitialized -Winit-self -Wmaybe-uninitialized

all:
	gcc $(FLAGS) str.c -c
	gcc $(FLAGS) ial.c -c
	gcc $(FLAGS) error.c -c
	gcc $(FLAGS) memory_manager.c -c
	gcc $(FLAGS) parser.c -c
	gcc $(FLAGS) lex.c -c
	gcc $(FLAGS) expr.c -c
	gcc $(FLAGS) keywords.c -c
	gcc $(FLAGS) stack.c -c
	gcc -o test str.o ial.o error.o memory_manager.o parser.o keywords.o lex.o expr.o stack.o
	
