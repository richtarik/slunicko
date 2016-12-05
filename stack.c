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

    *c=s->data[s->top];
}


void stackPop (IntStack* s ) {
/*   --------
** Odstraní prvek z vrcholu zásobníku.
*/
    if(stackEmpty(s))
    {
		error_f(ERROR_INTERN);
    }

    s->top--;
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

    s->top++;
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
    s->top = -1;
    memory_manager_free_one(s->data);
    s->data=NULL;
}

void VStackInit(VariableStack* s, unsigned int size) {
	s->top = -1;
	s->max = size;
	s->data = (T_variable*) memory_manager_malloc(sizeof(T_variable)*size);
}

int VStackEmpty(VariableStack* s) {
	return (s->top == -1) ? 1 : 0;
}

int VStackFull(VariableStack* s) {
	return (s->top == (int)s->max - 1) ? 1 : 0;
}

void VStackTop(VariableStack* s, T_variable* c) {

		if (VStackEmpty(s))
		{
      // pokus o TOP nad prazdnim stackom neukoncuje program len warrning chybova hlaska na stderr ze niekto je ... a vola Top nad prazdnym stackom
      // TODO
		// ZLE error_f(ERROR_INTERN);
		}

		(*c).type = ((s->data[s->top]).type);
		(*c).value = ((s->data[s->top]).value);
}


void VStackPop(VariableStack* s) {

	if (VStackEmpty(s))
	{
      // pokus o POP nad prazdnim stackom neukoncuje program len warrning chybova hlaska na stderr ze niekto je ... a popuje prazdny stack
      // TODO
		// ZLE error_f(ERROR_INTERN);
	}

	s->top--;
}

T_variable* VStackGet(VariableStack* s, int offset) {

    if(offset > s->top)
    {
        return NULL;
    }
    return (&(s->data[offset]));
}


void VStackSet(VariableStack* s, int offset, value_type type, union T_value value) {

	s->data[offset].type = type;
	s->data[offset].value = value;
}

void VStackPush(VariableStack* s, value_type type, union T_value value) {

	if (VStackFull(s))
	{
		s->max += s->max;
		s->data = (T_variable*) memory_manager_realloc(s->data, sizeof(T_variable)*(s->max));
	}

	s->top++;
	s->data[s->top].type = type;
	s->data[s->top].value = value;
}

void VStackDelete_and_free(VariableStack* s)
{
	s->top = -1;
	memory_manager_free_one(s->data);
	s->data = NULL;
}
