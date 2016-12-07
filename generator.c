///* File: generator.c         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "generator.h"

static unsigned int labelTemp = 1;
static unsigned int *label = &labelTemp;
static IntStack *func_stack;
static T_instr_list* iList;
static T_function_list* fList;

//Provadi generaci a optimalizaci vnitrniho kodu pro interpretaci
int generator(T_instr_list *L, bool isRoot) {
	if (!L) {
		return -1;
	}
	//printf("Vypis listu v generatoru\n");
    //list_print(L);
	if (isRoot) {
		iList = memory_manager_malloc(sizeof(T_instr_list));
		listInit(iList);
		fList = memory_manager_malloc(sizeof(T_function_list));
		fListInit(fList);
		func_stack = memory_manager_malloc(sizeof(IntStack));
		stackInit(func_stack, 50);
	}

    int i;
	int error = 0;
	T_address_code *T;
	T_address_code *S;
	T_address_code *R;
	T = memory_manager_malloc(sizeof(T_address_code));
	S = memory_manager_malloc(sizeof(T_address_code));
	R = memory_manager_malloc(sizeof(T_address_code));

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
			case T_UNM:
				listInsert(iList, T);
				break;

			//Logicke operace

			case T_AND:
			case T_OR:
			case T_NOT:
			    listInsert(iList, T);
				break;

			//Porovnavaci operace
			case T_EQUAL:
			case T_LEQUAL:
			case T_MEQUAL:
			case T_NEQUAL:
			case T_MORE:
			case T_LESS:
				listInsert(iList, T);
				break;

            //Zasobnikove instrukce
            case T_PUSH:
            case T_POP:
            case T_MOV:
                listInsert(iList, T);
                break;

			//Vstup a vystup

			case T_IIN:
			case T_DIN:
			case T_SIN:
			case T_OUT:
				listInsert(iList, T);
				break;

			//Vestavene funkce

			case T_LENGTH:
			case T_SUBSTR:
			case T_COMPARE:
			case T_FIND:
			case T_SORT:
				listInsert(iList, T);
				break;

			//Zalkadni "cykly"

			case T_IF:
				error = generator(T->address1, false);
				if (error) {
					listFree(L);
					listFree(iList);
					return error;
				}
				S->operation = T_JMPZD;
				S->result = *label;
				labelTemp++;
				listInsert(iList, S);
				error = generator(T->address2, false);
				if (error) {
					listFree(L);
					listFree(iList);
					return error;
				}
				if (T->result) {
					R->operation = T_JMPD;
					R->result = *label;
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
						return error;
					}
					R->operation = T_LABEL;
					listInsert(iList, R);
				}
				break;

			case T_WHILE:
				S->operation = T_LABEL;
				S->result = *label;
				labelTemp++;
				listInsert(iList, S);
				error = generator(T->address1, false);
				if (error) {
					listFree(L);
					listFree(iList);
					error_f(ERROR_INTERN);
					return error;
				}
				R->operation = T_JMPZD;
				R->result = *label;
				labelTemp++;
				listInsert(iList, R);
				error = generator(T->address2, false);
				if (error) {
					listFree(L);
					listFree(iList);
					error_f(ERROR_INTERN);
					return error;
				}
				S->operation = T_JMPU;
				listInsert(iList, S);
				R->operation = T_LABEL;
				listInsert(iList, R);
				break;

			//funkce

			case T_FUNC:
				i = (int)T->address1;
				i = fListFind(fList, i);
				if (i == -1) {
					fListInsert(fList, (int)T->address1, *label, 0);
					labelTemp++;
				}
				else {
					fList->Active->Function.iteration++;
				}
				R->operation = T_FSTART;
				R->address1 = i;
				R->address2 = fList->Active->Function.iteration;
				R->result = fList->Active->Function.label;
				listInsert(iList, R);
				if (i >= 1) {
					S->operation = T_FLABEL;
					S->address1 = fList->Active->Function.iteration;
					S->address2 = T->address2;
					S->result = fList->Active->Function.label + 1;
					listInsert(iList, S);
					break;
				}
				S->operation = T_FLABEL;
				S->address1 = 0;
				S->address2 = T->address2;
				S->result = *label;
				stackPush(func_stack, *label);
				labelTemp++;
				error = generator(T->address1, false);
				if (error) {
					listFree(L);
					listFree(iList);
					error_f(ERROR_INTERN);
					return error;
				}
				stackPop(func_stack);
				listInsert(iList, S);
				break;

			case T_RETURN:
				S->operation = T_FJMP;
				stackTop(func_stack, &i);
				S->result = i;
				listInsert(iList, S);
				break;

			default:
				listFree(L);
				return -1;
		}
		if (L->Active->next_item == NULL) {
			break;
		}
		listNext(L);
	}
	listFree(L);
	memory_manager_free_one(T);
	memory_manager_free_one(S);
	memory_manager_free_one(R);
	if (isRoot) {
        stackDelete_and_free(func_stack);

 //       printf("\n\nVypis listu2: \n");
//        list_print(iList);

		error = interpret(iList);
		if (error)
			error_f(ERROR_INTERN);
		return error;
	}
	else {
		return 0;
	}
}
