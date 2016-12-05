///* File: interpret.c         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 10.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "interpret.h"
#include "instrlist.h"
#include "generator.h"
#include "str.h"

//Provadi samotnou interpretaci predanych instrukci - pocetni operaca a instrukce skoku
int interpret(T_instr_list *L) {
    int i;
    double d;
    String* s;
	T_address_code *T;
	T_address_code *S;
	T_variable *A1;
	T_variable *A2;
	T_variable *A3;
	T_address *POM;
	IntStack* offset_stack;
	IntStack* func_stack;

	bool ZeroFlag = false;
	int frame = 0;
	bool jumpBack = false;
	bool isVoid = false;

	offset_stack = memory_manager_malloc(sizeof(IntStack));
	stackInit(offset_stack, 99);
	func_stack = memory_manager_malloc(sizeof(IntStack));
	stackInit(func_stack, 99);
	T = memory_manager_malloc(sizeof(T_address_code));
	S = memory_manager_malloc(sizeof(T_address_code));
	A1 = memory_manager_malloc(sizeof(T_variable));
	A2 = memory_manager_malloc(sizeof(T_variable));
	A3 = memory_manager_malloc(sizeof(T_variable));
	POM = memory_manager_malloc(sizeof(T_address));
	listFirst(L);

	while (1) {
		T = listGetItem(L);
		if (T == NULL) {
			return -1;
		}
		switch (T->operation) {

			//Instrukce pro aritmeticko operace
			case T_ADD:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}

				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int + A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int + A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double + A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double + A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == STRING && A2->type == STRING) {
					String* tmp;
					tmp = A1->value.value_String;
					strLoad(A3->value.value_String, tmp->str);
					tmp = A2->value.value_String;
					strCat(A3->value.value_String, tmp->str);
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
                    VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}

				break;

			case T_INC:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}

				if (A1->type == INT) {
					A1->value.value_int = A1->value.value_int + 1;
					ZeroFlag = !(A3->value.value_int == 0);
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A1->type, A1->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A1->type, A1->value);
				}

				break;

			case T_SUB:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int - A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int - A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double - A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double - A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_DEC:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					A1->value.value_int = A1->value.value_int - 1;
					ZeroFlag = !(A3->value.value_int == 0);
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A1->type, A1->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A1->type, A1->value);
				}
				break;

			case T_MUL:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int * A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int * A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double * A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double * A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_DIV:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						if (A2->value.value_int == 0) {
							error_f(ERROR_DIV_ZERO);
						}
						A3->value.value_int = A1->value.value_int / A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						if (fabs(A2->value.value_double) < 10e-7) {
							error_f(ERROR_DIV_ZERO);
						}
						A3->value.value_double = A1->value.value_int / A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						if (A2->value.value_int == 0) {
							error_f(ERROR_DIV_ZERO);
						}
						A3->value.value_double = A1->value.value_double / A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						if (fabs(A2->value.value_double) < 10e-7) {
							error_f(ERROR_DIV_ZERO);
						}
						A3->value.value_double = A1->value.value_double / A2->value.value_double;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_UNM:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					A1->value.value_int = -1 * A1->value.value_int;
				}
				else if (A1->type == DOUBLE) {
					A1->value.value_double = -1 * A1->value.value_double;
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A1->type, A1->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A1->type, A1->value);
				}
				break;

			//Logicke operace
			case T_AND:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int && A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int && A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int && A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double && A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double && A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double && A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool && A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool && A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool && A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_OR:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int || A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int || A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int || A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double || A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double || A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double || A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool || A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool || A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool || A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_NOT:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					A3->value.value_int = !(A1->value.value_int);
					ZeroFlag = !(A3->value.value_int == 0);
				}
				else if (A1->type == DOUBLE) {
					A3->value.value_double = !(A1->value.value_double);
					ZeroFlag = !(A3->value.value_double == 0);
				}
				else if (A1->type == BOOLEAN) {
					A3->value.value_bool = !(A1->value.value_bool);
					ZeroFlag = !(A3->value.value_bool == 0);
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			//Porovnavaci operace
			case T_EQUAL:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int == A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int == A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int == A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double == A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double == A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double == A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool == A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool == A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool == A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_LEQUAL:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int <= A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int <= A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int <= A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double <= A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double <= A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double <= A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool <= A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool <= A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool <= A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_MEQUAL:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int >= A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int >= A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int >= A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double >= A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double >= A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double >= A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool >= A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool >= A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool >= A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_NEQUAL:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int != A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int != A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int != A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double != A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double != A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double != A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool != A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool != A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool != A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_MORE:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int > A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int > A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int > A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double > A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double > A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double > A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool > A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool > A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool > A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			case T_LESS:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_int < A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_int < A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_int = A1->value.value_int < A2->value.value_bool;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == DOUBLE) {
					if (A2->type == INT) {
						A3->value.value_double = A1->value.value_double < A2->value.value_int;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_double < A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_double = A1->value.value_double < A2->value.value_bool;
						ZeroFlag = !(A3->value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1->type == BOOLEAN) {
					if (A2->type == INT) {
						A3->value.value_int = A1->value.value_bool < A2->value.value_int;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == DOUBLE) {
						A3->value.value_double = A1->value.value_bool < A2->value.value_double;
						ZeroFlag = !(A3->value.value_int == 0);
					}
					else if (A2->type == BOOLEAN) {
						A3->value.value_bool = A1->value.value_bool < A2->value.value_bool;
						ZeroFlag = !(A3->value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A3->type, A3->value);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A3->type, A3->value);
				}
				break;

			//Vestaveny funkce
			case T_IIN:
				i = strReadInt();
				POM = T->address1;
				A1->type = INT;
				A1->value.value_int = i;
				VStackSet(sVariableGlobal, 0, A1->type, A1->value);
				break;

			case T_DIN:
				d = strReadDouble();
				A1->type = DOUBLE;
				A1->value.value_double = d;
				VStackSet(sVariableGlobal, 0, A1->type, A1->value);
				break;

			case T_SIN:
				s = strReadString();
				POM = T->address1;
				A1->type = STRING;
				A1->value.value_String = s;
				VStackSet(sVariableGlobal, 0, A1->type, A1->value);
				break;

			case T_OUT:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				if (A1->type == INT) {
					printf("%d", A1->value.value_int);
				}
				else if (A1->type == DOUBLE) {
					printf("%g", A1->value.value_double);
				}
				else if (A1->type == STRING) {
					strPrintStr(A1->value.value_String);
				}
				else {
					return -1;
				}
				break;

			case T_LENGTH:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
                    A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				i = strLength(A1->value.value_String);
				A2->type = INT;
				A2->value.value_int = i;
				VStackSet(sVariableGlobal, 0, A2->type, A2->value);
				break;

			case T_SUBSTR:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->result;
				if (POM->isGlobal) {
					A3 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				s = strSubstr(A1->value.value_String, A2->value.value_int, A3->value.value_int);
				A1->type = STRING;
				A1->value.value_String = s;
				VStackSet(sVariableGlobal, 0, A1->type, A1->value);
				break;

			case T_COMPARE:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				i = strCompare(A1->value.value_String, A2->value.value_String);
				A3->type = INT;
				A3->value.value_int = i;
				VStackSet(sVariableGlobal, 0, A3->type, A3->value);
				break;

			case T_FIND:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					A2 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				i = find(A1->value.value_String, A2->value.value_String);
				A3->type = INT;
				A3->value.value_int = i;
				VStackSet(sVariableGlobal, 0, A3->type, A3->value);
				break;

			case T_SORT:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				s = sort(A1->value.value_String);
				A2->type = STRING;
				A2->value.value_String = s;
				VStackSet(sVariableGlobal, 0, A2->type, A2->value);
				break;

			//Zasobnikove funkce
			case T_PUSH:
				POM = T->result;
				A1 = T->address1;
				if (POM->isGlobal) {
					VStackPush(sVariableGlobal, A1->type, A1->value);
					printf("\n\nPUSH - Vypis globalneho stacku: \n");
                    print_VStack_data(sVariableGlobal,stderr);
				}
				else {
					VStackPush(sVariableLocal, A1->type, A1->value);
					printf("\n\nPUSH - Vypis lokalneho stacku: \n");
                    print_VStack_data(sVariableLocal,stderr);
				}
				break;

			case T_POP:
				POM = T->result;
				if (POM->isGlobal) {
					VStackPop(sVariableGlobal);
					printf("\n\nPOP - Vypis globalneho stacku: \n");
                    print_VStack_data(sVariableGlobal,stderr);
				}
				else {
					VStackPop(sVariableLocal);
					printf("\n\nPOP - Vypis lokalneho stacku: \n");
                    print_VStack_data(sVariableLocal,stderr);
				}
				break;

			case T_MOV:
				POM = T->address1;
				if (POM->isGlobal) {
					A1 = VStackGet(sVariableGlobal, POM->offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, frame + POM->offset);
				}
				POM = T->address2;
				if (POM->isGlobal) {
					VStackSet(sVariableGlobal, POM->offset, A1->type, A1->value);
					printf("\n\nMOV - Vypis lokalneho stacku: \n");
                    print_VStack_data(sVariableGlobal,stderr);
				}
				else {
					VStackSet(sVariableLocal, frame + POM->offset, A1->type, A1->value);
					printf("\n\nMOV - Vypis lokalneho stacku: \n");
                    print_VStack_data(sVariableLocal,stderr);
				}
				break;

			//Skokove instrukce pro if a cykly
			case T_LABEL:
				break;

			case T_JMPZD:
				if (ZeroFlag) {
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
				ZeroFlag = false;
				break;

			case T_JMPU:
				listFirst(L);
				while (1) {
					S = listGetItem(L);
					if (S->operation == T_LABEL && S->result == T->result) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
					listNext(L);
				}
				ZeroFlag = false;
				break;

			//Instrukce pro funkcnost uzivatelskych funkci
			case T_FSTART:
				if (T->address1 != -1) {
					listFirst(L);
					while (1) {
						S = listGetItem(L);
						if (S->operation == T_FSTART && S->result == T->result) {
                            break;
						}
						if (L->Active->next_item == NULL) {
                            return -1;
						}
						listNext(L);
					}
				}
				stackPush(func_stack, (int) T->address2);
			    frame = sVariableLocal->top + 1;
				stackPush(offset_stack, frame);
				break;

			case T_FJMP:
				stackTop(func_stack, &i);
				stackPop(func_stack);
				if (i != 0) {
					listFirst(L);
				}
				while (1) {
					listNext(L);
					S = listGetItem(L);
					if (S->operation == T_FLABEL && S->result == T->result && S->address1 == i) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
				}
				jumpBack = true;
				continue;

			case T_FLABEL:
			    isVoid = T->address2;
			    if (!jumpBack && !isVoid) {
                    error_f(8);
			    }
				if (!jumpBack) {
					stackTop(func_stack, &i);
					stackPop(func_stack);
					if (i != 0) {
						listFirst(L);
						while (1) {
							S = listGetItem(L);
							if (S->operation == T_FLABEL && S->result == T->result && S->address1 == i) {
								break;
							}
							if (!(L->Active->next_item)) {
								return -1;
							}
							listNext(L);
						}
						listNext(L);
					}
				}
			    jumpBack = false;
			    while (sVariableLocal->top > frame) {
                    VStackPop(sVariableLocal);
			    }
			    //if(!VStackEmpty(L)) //pri voide si musim popnut na inom mieste
                //    VStackPop(sVariableLocal); //implementacny detail
				stackPop(offset_stack);
				if (stackEmpty(offset_stack)) {
					frame = 0;
				}
				else {
					stackTop(offset_stack, &frame);
				}
				break;

			default:
				return -1;

		}
		if (L->Active->next_item == NULL) {
            memory_manager_free_one(T);
			memory_manager_free_one(S);
			memory_manager_free_one(A1);
			memory_manager_free_one(A2);
			memory_manager_free_one(A3);
			return 0;
		}
		listNext(L);
	}
}
