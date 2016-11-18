///* File: interpret.h         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 10.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "struct.h"
#include "stack.h"
#include "ial.h"

void VStackInit(VariableStack* s, unsigned int size);
int VStackEmpty(VariableStack* s);
int VStackFull(VariableStack* s);
void VStackTop(VariableStack* s, int* c);
void VStackPop(VariableStack* s);
T_variable VStackGet(VariableStack* s, int offset);
void VStackSet(VariableStack* s, int offset, T_variable data);
void VStackPush(VariableStack* s, T_variable data);
void VStackDelete_and_free(VariableStack* s);
int interpret(T_instr_list *L, VariableStack vStack);