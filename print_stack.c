#include "stack.h"
#include "struct.h"
#include "print_stack.h"
#include <stdbool.h>

void print_stack_data(IntStack *s, FILE* suborik)
{
    int i = 0;

    while(s->top - i >= 0)
    {
        fprintf(suborik," %c ", int_to_operator(s->data[i]) );
        i++;
    }
    fprintf(suborik,"\n");
}


char int_to_operator(int i)
{
  switch(i)
  {
    case 0 : return'-';    // / Unary minus
    //case  1: return'=';    // Assignment
    case  1: return'+';    // Addition
    case  2: return'-';    // Subtraction
    case  3: return'*';   // Multiplication
    case  4: return'/';    // Division
    case  5: return'%';   // Modulo

    case  6: return'I';    //  Increment
    case  7: return'D';   //  Decrement

    case  8: return'E';    //  Equal
    case 9: return'Q';   // // Not equal
    case 10: return'>';   // / Greater
    case 11 : return'<';    //  Less
    case 12 : return'G';    // / Greater or equal
    case 13 : return'L';    // / Less or equal

    case 14: return'|';   // // Or
    case 15 : return'&';    // / And
    case 16 : return'!';    //  Not

    case 17 : return'(';    //  Parenthesis left
    case 18 : return')';    //  Parenthesis right
    case 19: return',';      // / ID

    case 20: return'i';      // / ID
    case 21 : return'f';      //  function
    case 22 : return'$';    //  Dollar
    case 25: return'n';      // Nonterminal
    case 30: return'=';
    case 31: return'<';
  }
  return 'F';
}


void print_table(hash_table_ptr ht)
{
    int item_count=0;
    if(ht != NULL)
    {
        for( unsigned i = 0; i < ht->size ; i++ )
        {
            ht_table_item_ptr pom_table= ht->table_item[i];
            while(pom_table != NULL)
            {

                printf("Table_indx: %d \t Item:%d \t Key: %s\n",i,item_count,pom_table->key);
                if(pom_table->func == NULL)
                {
                    int initt=pom_table->var->inicialized;
                    //int defined=ht->table_item[i]->var->defined;
                    int typ=pom_table->var->type;
                    printf("VARIABLE: \t \tinit:%d \t token_type:",initt,typ);
                    f_print_type(typ);
                    printf("\t offset: %d", pom_table->var->offset);
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
                        printf(" %s,",pom_fnc_param->name);
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
        case 49:
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
        printf("num:%d\t instruction/operation:",i);
        switch(operations)
        {
            case token_add:
                fprintf(stderr," add\n");
                break;
            case token_inc:
                fprintf(stderr," inc\n");
                break;
            case token_sub:
                fprintf(stderr," sub\n");
                break;
            case token_dec:
                fprintf(stderr," dec\n");
                break;
            case token_mul:
                fprintf(stderr," mul\n");
                break;
            case token_div:
                fprintf(stderr," div\n");
                break;
            case token_and:
                fprintf(stderr," and\n");
                break;
            case token_or:
                fprintf(stderr," or\n");
                break;
            case token_not:
                fprintf(stderr," not \n");
                break;
            case token_equ:
                fprintf(stderr," equ\n");
                break;
            case token_loe:
                fprintf(stderr," loe\n");
                break;
            case token_goe:
                fprintf(stderr," goe\n");
                break;
            case token_neq:
                fprintf(stderr," neq\n");
                break;
            case token_gre:
                fprintf(stderr," gre\n");
                break;
            case token_les:
                fprintf(stderr," les\n");
                break;
            case 100:
                fprintf(stderr," T_IN \n");
                break;
            case 101:
                fprintf(stderr," T_out \n");
                break;
            case token_if:
                fprintf(stderr," if \n");
                break;
            case token_while:
                fprintf(stderr," while\n");
                break;
            case 99:
                fprintf(stderr," Function\n");
                break;
            case token_return:
                fprintf(stderr," return\n");
                break;
            case token_func:
                fprintf(stderr," funct\n");
                break;
            case token_label:
                fprintf(stderr," label\n");
                break;
            case token_jmpzd:
                fprintf(stderr," jmpzd\n");
                break;
            case token_jmpd:
                fprintf(stderr," jmpd\n");
                break;
            case token_fstart:
                fprintf(stderr," fstart\n");
                break;
            case token_fjmp:
                fprintf(stderr," jmp\n");
                break;
            case token_flabel:
                fprintf(stderr," flabel \n");
                break;
            case token_pull:
                fprintf(stderr," POP \n");
                break;
            case token_push:
                fprintf(stderr," PUSH \n");
                break;
            case token_mov:
                fprintf(stderr," MOVE \n");
                break;
            default:
                fprintf(stderr," I dont know \n");
                break;
        }
        fprintf(stderr,"Adresa1: %d", tmp->Instruction.address1);
        fprintf(stderr," ,Adresa2: %d", tmp->Instruction.address2);
        fprintf(stderr," ,Adresa3: %d\n\n", tmp->Instruction.result);
        tmp = (tmp->next_item);
        i++;
    }
    return;
}

void print_VStack_data(VariableStack *s,FILE* suborik)
{
    int i = 0;

    while(s->top - i >= 0)
    {
        fprintf(suborik,"i:%d type:",i);
        switch( (s->data[i].type) )
        {
            case INT:
                fprintf(suborik,"INT \t value: %d", s->data[i].value);
                break;
            case DOUBLE:
                fprintf(suborik,"DOUBLE\t value: %f", s->data[i].value);
                break;
            case STRING:
                fprintf(suborik,"STRING\t value: %s", s->data[i].value);
                break;
            case BOOLEAN:
                fprintf(suborik,"BOOLEAN value: %d", s->data[i].value);
                break;
            default:
                fprintf(suborik,"Defauuult value: %d num_Type:%d", s->data[i].value,s->data[i].type);
                break;
        }

        i++;
        fprintf(suborik,"\n");
    }
    fprintf(suborik,"\n");
}
