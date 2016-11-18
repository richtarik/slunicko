#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"
#include "error.h"
#include "struct.h"

//#define size 20

/* ADT zásobník */
typedef struct
{
    int* data;
    unsigned max;
    int top;        /* index prvku na vrcholu zásobníku */
} IntStack;

/* Hlavičky funkcí pro práci se zásobníkem. */
void stackInit (IntStack* s, unsigned size );
int stackEmpty (IntStack* s );
int stackFull (IntStack* s );
void stackTop (IntStack* s, int* c );
void stackPop (IntStack* s );
void stackPush (IntStack* s, int c );
void print_stack_data_i(IntStack *s);
void stackDelete_and_free(IntStack* s);

#endif

void VStackInit(VariableStack* s, unsigned int size);
int VStackEmpty(VariableStack* s);
int VStackFull(VariableStack* s);
void VStackTop(VariableStack* s, T_variable* c);
void VStackPop(VariableStack* s);
T_variable* VStackGet(VariableStack* s, int offset);
void VStackDelete_and_free(VariableStack* s);

