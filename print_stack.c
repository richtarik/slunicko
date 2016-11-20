#include "stack.h"
#include "struct.h"
#include "print_stack.h"
#include <stdbool.h>

void print_stack_data(IntStack *s)
{
    int i = 0;

    while(s->top - i >= 0)
    {
        printf(" %c ", int_to_operator(s->data[i]) );
        i++;
    }
    printf("\n");
}


char int_to_operator(int i)
{
  switch(i)
  {
    case 0 : return'-';    // / Unary minus
    case  1: return'=';    // Assignment
    case  2: return'+';    // Addition
    case  3: return'-';    // Subtraction
    case  4: return'*';   // Multiplication
    case  5: return'/';    // Division
    case  6: return'%';   // Modulo

    case  7: return'I';    //  Increment
    case  8: return'D';   //  Decrement

    case  9: return'E';    //  Equal
    case 10: return'Q';   // // Not equal
    case 11: return'>';   // / Greater
    case 12 : return'<';    //  Less
    case 13 : return'G';    // / Greater or equal
    case 14 : return'L';    // / Less or equal

    case 15: return'|';   // // Or
    case 16 : return'&';    // / And
    case 17 : return'!';    //  Not

    case 18 : return'(';    //  Parenthesis left
    case 19 : return')';    //  Parenthesis right
    case 20: return',';      // / ID

    case 21: return'i';      // / ID
    case 22 : return'f';      //  function
    case 23 : return'$';    //  Dollar
    case 25: return'n';      // Nonterminal
    case 24: return'.';      // Dot
    case 26: return'=';
    case 27: return'<';
  }
  return 'F';
}


void print_table(hash_table_ptr ht)
{
    int item_count=0;
    if(ht != NULL)
    {
unsigned i;
        for( i = 0; i < ht->size ; i++ )
        {
            ht_table_item_ptr pom_table= ht->table_item[i];
            while(pom_table != NULL)
            {

                printf("Table_indx: %d \t Item:%d \t Key: %s\n",i,item_count,pom_table->key);
                if(pom_table->func == NULL)
                {
                    int initt=ht->table_item[i]->var->inicialized;
                    //int defined=ht->table_item[i]->var->defined;
                    int typ=ht->table_item[i]->var->type;
                    printf("VARIABLE: \t \tinit:%d \t token_type:",initt,typ);
                    f_print_type(typ);
                    printf("\n");
                }
                else if(pom_table->var == NULL)
                {
                    //ht->table_item[i]->func->param->name;
                    int typ=pom_table->func->type;
                    printf("FUNCTION: \t param: ");
                    ht_params_ptr pom_fnc_param= pom_table->func->param;
                    printf(" token_type: ");
                    f_print_type(typ);
                    while( pom_fnc_param != NULL)
                    {
                        printf(" %s,",ht->table_item[i]->func->param->name);
                        pom_fnc_param= pom_fnc_param->next;
                    }
                    printf("\n");
                }
                item_count++;
                pom_table= pom_table->next;

            }
            item_count=0;
        }
    }
}

void f_print_type(int TOKENIKTYPE)
{
    switch(TOKENIKTYPE)
    {
        case 37:
            printf("void\t type_num: %d\n",TOKENIKTYPE);
            break;
        case 38:
            printf("double\t type_num: %d\n",TOKENIKTYPE);
            break;
        case 39:
            printf("int\t type_num: %d\n",TOKENIKTYPE);
            break;
        case 40:
            printf("String\t type_num: %d\n",TOKENIKTYPE);
            break;
        case 41:
            printf("boolean\t type_num: %d\n",TOKENIKTYPE);
            break;
        default:
            printf("Something Wrong type_num: %d\n",TOKENIKTYPE);
            break;
    }
}



void list_print(T_instr_list *L)
{
    T_item_list *tmp;
    tmp = L->First;
    int i=0;
    int operations;
    while (tmp != NULL)
    {
        operations=(tmp->Instruction.operation);
        printf("ins:%d\t operations:%c\n",i,int_to_operator(operations+1));
        tmp= (tmp->next_item);
        i++;
    }
    return;
}

void print_VStack_data(VariableStack *s)
{
    int i = 0;

    while(s->top - i >= 0)
    {
        printf("i:%d type:",i);
        switch( (s->data[i].type) )
        {
            case INT:
                printf("INT \t value: %d", s->data[i].value);
                break;
            case DOUBLE:
                printf("DOUBLE\t value: %f", s->data[i].value);
                break;
            case STRING:
                printf("STRING\t value: %s", s->data[i].value);
                break;
            case BOOLEAN:
                printf("BOOLEAN value: %d", s->data[i].value);
                break;
        }

        i++;
        printf("\n");
    }
    printf("\n");
}
