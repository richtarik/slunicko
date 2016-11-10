///* File: generator.h         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "struct.h"

void stackInit(int_stack *S);
void stackPush(int_stack *S, int *item);
void stackRemove(int_stack *S);
int stackTop(int_stack *S);
bool stackEmpty(int_stack *S);
int stackPop(int_stack *S);

int generator(T_instr_list *L, bool isRoot);