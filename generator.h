#include "struct.h"

typedef struct {
	int *item;
	int top;
}int_stack;

void stackInit(int_stack *S);
void stackPush(int_stack *S, int *item);
void stackRemove(int_stack *S);
int stackTop(int_stack *S);
bool stackEmpty(int_stack *S);
int stackPop(int_stack *S);

int generator(T_instr_list *L, bool isRoot);