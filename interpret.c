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
	s->data = malloc(sizeof(ht_item_var_ptr)*size);

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

ht_item_var_ptr VStackGet(VariableStack* s, int offset) {
	return s->data[offset];
}

void VStackSet(VariableStack* s, int offset, ht_item_var_ptr data) {
	s->data[offset] = data;
}

void VStackPush(VariableStack* s, ht_item_var_ptr data) {
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
int interpret(T_instr_list *L, VariableStack vStack) {
	bool Flag = false;
	int offset = 0;
	T_address_code T = malloc(sizeof(T_address_code));
	T_address_code S = malloc(sizeof(T_address_code));
	ht_item_var_ptr A1 = malloc(sizeof(ht_item_var_ptr));
	ht_item_var_ptr A2 = malloc(sizeof(ht_item_var_ptr));
	ht_item_var_ptr A3 = malloc(sizeof(ht_item_var_ptr));
	listFirst(L);

	while (1) {
		T = listGetItem(L);
		if (T == NULL) {
			return -1;
		}
		switch (T->operation) {

			//Aritmeticke operace
			case T_ADD:
				A1 = VStackGet(vStack, offset + T->address1);
				A2 = VStackGet(vStack, offset + T->address2);
				A3 = VStackGet(vStack, offset + T->result);
				if (A1->type == token_int) {
					if (A2->type == token_int) {
						A3->value->value_int = A1->type->value_int + A2->type->value_int;
					}
					else if (A2->type == token_double) {
						A3->value->value_double = A1->type->vaule_int + A2->type->value_double;
					}
					else {
						return -1;
					}
				}
				else if (A1->type == token_double) {
					if (A2->type == token_int) {
						A3->value->value_double = A1->type->value_double + A2->type->value_int;
					}
					else if (A2->type == token_double) {
						A3->value->valee_double = A1->type->value_double + A2->type->value_double;
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, offset + T->result, A3);
				break;

			case T_INC:
				A1 = VStackGet(vStack, offset + T->address1);
				if (A1->type == token_int) {
					A1->value->value_int = A1->value->value_int + 1;
				}
				else {
					return -1;
				}
				VStackSet(vStack, offset + T->address1, A1);
				break;

			case T_SUB:
				A1 = VStackGet(vStack, offset + T->address1);
				A2 = VStackGet(vStack, offset + T->address2);
				A3 = VStackGet(vStack, offset + T->result);
				if (A1->type == token_int) {
					if (A2->type == token_int) {
						A3->value->value_int = A1->type->value_int - A2->type->value_int;
					}
					else if (A2->type == token_double) {
						A3->value->value_double = A1->type->vaule_int - A2->type->value_double;
					}
					else {
						return -1;
					}
				}
				else if (A1->type == token_double) {
					if (A2->type == token_int) {
						A3->value->value_double = A1->type->value_double - A2->type->value_int;
					}
					else if (A2->type == token_double) {
						A3->value->valee_double = A1->type->value_double - A2->type->value_double;
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, offset + T->result, A3);
				break;

			case T_DEC:
				A1 = VStackGet(vStack, offset + T->address1);
				if (A1->type == token_int) {
					A1->value->value_int = A1->value->value_int - 1;
				}
				else {
					return -1;
				}
				VStackSet(vStack, offset + T->address1, A1);
				break;

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