///* File: interpret.h         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 10.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "struct.h"
#include "ial.h"

typedef struct {
	ht_item_func_ptr* data;
	unsigned int max;
	int top;
} VariableStack;

void VStackInit(VariableStack* s, unsigned int size);
int VStackEmpty(VariableStack* s);
int VStackFull(VariableStack* s);
void VStackTop(VariableStack* s, int* c);
void VStackPop(VariableStack* s);
ht_item_func_ptr VStackGet(VariableStack* s, int offset);
void VStackPush(VariableStack* s, ht_item_func_ptr data);
void VStackDelete_and_free(VariableStack* s);
int interpret(T_instr_list *L);