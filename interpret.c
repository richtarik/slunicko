///* File: interpret.c         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 10.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "interpret.h"
#include "instrList.h"

void VStackInit(VariableStack* s, unsigned int size) {
	s->top = -1;
	s->max = size;
	s->data = malloc(sizeof(ht_item_func_ptr)*size);

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

ht_item_func_ptr VStackGet(VariableStack* s, int offset) {
	return s->data[offset];
}

void VStackPush(VariableStack* s, ht_item_func_ptr data) {
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

//Provadi samotnou interpretaci predanych instrukci - pocetni operaca a instrukce skoku
int interpret(T_instr_list *L) {
	bool Flag = false;
	T_address_code T = malloc(sizeof(T_address_code));
	T_address_code S = malloc(sizeof(T_address_code));
	listFirst(L);

	while (1) {
		T = listGetItem(L);
		if (T == NULL) {
			return -1;
		}
		switch (I->operation) {
			/*Nejdøív musí být všechny aritmetický a podobný další všechny operace, èeká se na strukturu z parseru*/

			case T_LABEL:
			case T_FLABEL:
				break;

			case T_JMPZD:
				if (Flag) {
					break;
				}

			case T_JMPD:
				while (1) {
					listNext(L);
					S = listGetItem(L);
					if (S->operation == T_LABEL && S->result == T->result) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
				}
				flag = false;
				break;

			case T_FJMP:
				while (1) {
					listNext(L);
					S = listGetItem(L);
					if (S->operation == T_LABEL && S->result == T->result) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
				}
				//vkládání honot do returnu, èekám na struktury
				break;

			case default:
				return -1;

		}
		if (L->Active->next_item == NULL) {
			return 0;
		}
		listNext(L);
	}
}