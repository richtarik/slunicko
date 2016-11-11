#include "../../stack.h"

#include <stdio.h>
#include <stdlib.h>

int main ()
{
	unsigned size=20;

	IntStack* T_stack=malloc(sizeof(T_stack));
	stackInit(T_stack,size);

	printf("test01: Push 20\n");
	stackPush(T_stack,size);
	print_stack_data(T_stack);

	printf("test02: Push 2\n");
	stackPush(T_stack,2);
	print_stack_data(T_stack);

	printf("test03: Push 0-9\n");
	for(int i=0; i<10; i++)
	{
		stackPush(T_stack,i);
	}
	print_stack_data(T_stack);

	printf("test04: 5*Pop\n");
	for(int i=0; i<5; i++)
	{
		stackPop(T_stack);
	}
	print_stack_data(T_stack);

}
