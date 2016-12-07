#include <stdio.h>
#include <stdlib.h>

#include "ial.h"



void print_stack_data(IntStack *s,FILE *suborik);
char int_to_operator(int i);
void f_print_type(int TOKENIKTYPE);
void print_table(hash_table_ptr ht);
void list_print(T_instr_list *L);
void print_VStack_data(VariableStack *s,FILE* suborik);
