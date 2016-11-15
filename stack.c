#include "stack.h"

void stackInit (IntStack* s, unsigned size) {
/*   ---------
** Provede inicializaci zásobníku - nastaví vrchol zásobníku.
*/
    s->top = -1;
    s->max = size;
    s->data = malloc(sizeof(int)*size);

    if(s->data == NULL)
    {
        fprintf(stderr,"Error init allock stack \n"); // TODO
        return;
    }
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
    return (s->top == s->max-1) ? 1 : 0;
}

void stackTop (IntStack* s, int* c ) {
/*   --------
** Vrací znak z vrcholu zásobníku prostřednictvím parametru c.
*/
    if(stackEmpty(s))
    {
         fprintf(stderr,"Error empty stack - call stackTop\n"); // TODO
        return;
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
        fprintf(stderr,"Error empty stack - call stackpop\n"); // TODO
        return;
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
        //stackError(SERR_PUSH);
        s->data= (int*)realloc(s->data, sizeof(int)*(s->max + s->max) );
        if( s == NULL)
        {
            fprintf(stderr,"Error realloc stack \n"); // TODO
            return;
        }
        s->max+=s->max;
    }

    s->top+=1;
    s->data[s->top]=value;
    return;
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

int stackGet(IntStack* s) {
	return s->data[s->top];
}

int stackGetAndPop(IntStack* s) {
	int c = stackGet(s);
	stackPop(s);
	return c;
}


void stackDelete_and_free(IntStack* s)
{
    if(stackEmpty(s))
    {
        //stackError(SERR_PUSH);
        ;
    }
    else
    {
        while( !stackEmpty(s) )
        {
            stackPop(s);
        }
    }
    free(s->data);
    s->data=NULL;
    return;
}

