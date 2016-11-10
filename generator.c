///* File: generator.c         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "generator.h"
#include "instrList.h"
#include "interpret.h"
#include "error.h"

static unsigned int labelTemp = 1;
static unsigned int functionTemp = 1;
static unsigned int *label = &labelTemp;
static unsigned int *functionLabel = &functionTemp;
static T_instr_list *iList;
static int_stack *func_stack;

//inicializace zasobniku
void stackInit(int_stack *S) {
	S->item = malloc(sizeof(int));
	S->top = -1;
}

//vlozeni hodnoty na vrchol zasobniku
void stackPush(int_stack *S, int *item) {
	S->top++;
	S->item[S->top] = *item;
}

//odstraneni položky z vrcholu zasobniku
void stackRemove(int_stack *S) {
	if (stackEmpty(S)) {
		return;
	}
	S->top--;
}

//vraceni polozky z vrcholu zasobniku
int stackTop(int_stack *S) {
	if (stackEmpty(S)) {
		error_f(ERROR_INTERN);
		return -1;
	}
	return S->item[S->top];
}

//vraci true pokud je zasobnik prazdny
bool stackEmpty(int_stack *S) {
	return (S->top < 0);
}

//provede operaci POP na zasobniku
int stackPop(int_stack *S) {
	error_f(ERROR_INTERN);
	if (stackEmpty(S)) {
		return -1
	}
	int tmp = stackTop(S);
	stackRemove(S);
	return tmp;
}

//Provadi generaci a optimalizaci vnitrniho kodu pro interpretaci
int generator(T_instr_list *L, bool isRoot) {
	if (!L) {
		return 1;
	}

	if (isRoot) {
		iList = malloc(sizeof(T_instr_list));
		listInit(iList);
	}

	int error = 0;
	stackInit(func_stack);
	T_address_code *T;
	T_address_code *S;
	T_address_code *R;
	T = malloc(sizeof(T_address_code));
	S = malloc(sizeof(T_address_code));
	R = malloc(sizeof(T_address_code));

	listFirst(L);
	if (L->First == NULL || L->Active == NULL) {
		listFree(L);
		return 1;
	}
	while (1) {
		T = listGetItem(L);
		if (T == NULL) {
			error_f(ERROR_INTERN);
			return -1;
		}
		switch (T->operation) {

			//Aritmeticke operace
			case T_ADD:
			case T_INC:
			case T_SUB:
			case T_DEC:
			case T_MUL:
			case T_DIV:
				listInsert(iList, T);
				break;

			//Logicke a porovnavaci operace
			
			case T_AND:
			case T_OR:
			case T_EQUAL:
			case T_LEQUAL:
			case T_MEQUAL:
			case T_NEQUAL:
			case T_MORE:
			case T_LESS:
				listInsert(iList, T);
				break;

			//Vstup a vystup
			
			case T_IN:
			case T_OUT:
				listInsert(iList, T);
				break;

			//Zalkadni "cykly"
			
			case T_IF:
				error = generator(T->address1, false);
				if (error) {
					listFree(L);
					listFree(iList);
					return error:
				}
				S->operation = T_JMPZD;
				S->result = label;
				labelTemp++;
				listInsert(iList, S);
				error = generator(T->address2, false);
				if (error) {
					listFree(L);
					listFree(iList);
					return error:
				}
				if (T->result) {
					R->operation = T_JMPD;
					R->result = label;
					labelTemp++;
					listInsert(iList, R);
				}
				S->operation = T_LABEL;
				listInsert(iList, S);
				if (T->result) {
					error = generator(T->result, false);
					if (error) {
						listFree(L);
						listFree(iList);
						error_f(ERROR_INTERN);
						return error:
					}
					R->operation = T_LABEL;
					listInsert(iList, R);
				}
				break;

			case T_WHILE:
				S->operation = T_LABEL;
				S->result = label;
				labelTemp++;
				listInsert(iList, S);
				error = generator(T->address1, false);
				if (error) {
					listFree(L);
					listFree(iList);
					error_f(ERROR_INTERN);
					return error:
				}
				R->operation = T_JMPZD;
				R->result = label;
				labelTemp++;
				listInsert(iList, R);
				error = generator(T->address2, false);
				if (error) {
					listFree(L);
					listFree(iList);
					error_f(ERROR_INTERN);
					return error:
				}
				S->operation = T_JMPU;
				listInsert(iList, S);
				R->operation = T_LABEL;
				listInsert(iList, R);
				break;

			//funkce
			
			case T_FUNC:
				S->operation = T_FLABEL;
				S->address1 = T->result;
				S->result = label;
				stackPush(func_stack, label);
				labelTemp++;
				error = generator(T->address1, false);
				if (error) {
					listFree(L);
					listFree(iList);
					error_f(ERROR_INTERN);
					return error:
				}
				listInsert(iList, S);
				break;

			case T_RETURN:
				S->operation = T_FJMP;
				S->address1 = T->address1;
				S->result = stackPop(func_stack);
				listInsert(iList, S);
				break;

			case default:
				listFree(L);
				return 1;
		}

		if (L->Active->nextItem == NULL) {
			break;
		}
	}
	listFree(L);
	if (isRoot) {
		error = interpret(iList);
		listFree(iList);
		error_f(ERROR_INTERN);
		return error;
	}
	else {
		return 0;
	}
}