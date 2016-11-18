#include "stack.h"

void stackInit (IntStack* s, unsigned size) {
/*   ---------
** Provede inicializaci zásobníku - nastaví vrchol zásobníku.
*/
    s->top = -1;
    s->max = size;
    s->data = memory_manager_malloc(sizeof(int)*size);
}

int stackEmpty (IntStack* s ) {
/*  ----------
** Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
*/
    return (s->top == -1) ? 1 : 0;
}

int stackFull (IntStack* s ) {
/*  ---------
** Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
*/

	// max je unsigned!!
    return (s->top == (int)s->max-1) ? 1 : 0;
}

void stackTop (IntStack* s, int* c ) {
/*   --------
** Vrací znak z vrcholu zásobníku prostřednictvím parametru c.
*/
    if(stackEmpty(s))
    {
		error_f(ERROR_INTERN);
    }
    else
    {
        *c=s->data[s->top];
        return;
    }
}


void stackPop (IntStack* s ) {
/*   --------
** Odstraní prvek z vrcholu zásobníku.
*/
    if(stackEmpty(s))
    {
		error_f(ERROR_INTERN);
    }
    else
    {
        //s->arr[s->top]=0; // todo
        s->top-=1;
        return;
    }
}


void stackPush (IntStack* s, int value ) {
/*   ---------
** Vloží znak na vrchol zásobníku.
*/
    if(stackFull(s))
    {
        s->data= (int*)memory_manager_realloc(s->data, sizeof(int)*(s->max + s->max) );
        
        s->max+=s->max;
    }

    s->top+=1;
    s->data[s->top]=value;
}

void print_stack_data_i(IntStack *s)
{
    int i = 0;

    if(!stackEmpty(s))
    {
        while(s->top - i >= 0)
        {
            printf("%d ", s->data[i]);
            i++;
        }
        printf("\n");
    }
}

void stackDelete_and_free(IntStack* s)
{
    if(stackEmpty(s))
    {
		return;
    }
   
	while( !stackEmpty(s) )
    {
		stackPop(s);
    }
    
    memory_manager_free_one(s->data);
    s->data=NULL;
}

void VStackInit(VariableStack* s, unsigned int size) {
	s->top = -1;
	s->max = size;
	s->data = malloc(sizeof(T_variable)*size);

	if (s->data == NULL)
	{
		fprintf(stderr, "Error init allock stack \n"); // TODO
		return;
	}
}

int VStackEmpty(VariableStack* s) {
	return (s->top == -1) ? 1 : 0;
}

int VStackFull(VariableStack* s) {
	return (s->top == s->max - 1) ? 1 : 0;
}

void VStackTop(VariableStack* s, int* c) {
	*/
		if (stackEmpty(s))
		{
			fprintf(stderr, "Error empty stack - call stackTop\n"); // TODO
			return;
		}
		else
		{
			*c = s->data[s->top];
			return;
		}
}


void VStackPop(VariableStack* s) {
	if (stackEmpty(s))
	{
		fprintf(stderr, "Error empty stack - call stackpop\n"); // TODO
		return;
	}
	else
	{
		//s->arr[s->top]=0; // todo
		s->top -= 1;
		return;
	}
}

T_variable VStackGet(VariableStack* s, int offset) {
	return s->data[offset];
}

void VStackSet(VariableStack* s, int offset, T_variable data) {
	s->data[offset] = data;
}

void VStackPush(VariableStack* s, T_variable data) {
	if (stackFull(s))
	{
		//stackError(SERR_PUSH);
		s->data = (int*)realloc(s->data, sizeof(int)*(s->max + s->max));
		if (s == NULL)
		{
			fprintf(stderr, "Error realloc stack \n"); // TODO
			return;
		}
		s->max += s->max;
	}

	s->top += 1;
	s->data[s->top] = data;
	return;
}

void VStackDelete_and_free(VariableStack* s)
{
	if (stackEmpty(s))
	{
		//stackError(SERR_PUSH);
		;
	}
	else
	{
		while (!stackEmpty(s))
		{
			stackPop(s);
		}
	}
	free(s->data);
	s->data = NULL;
	return;
}