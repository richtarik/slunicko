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
#include "generator.h"

//Provadi samotnou interpretaci predanych instrukci - pocetni operaca a instrukce skoku
int interpret(T_instr_list *L, VariableStack vStack) {
	bool ZeroFlag = false;
	int frame = 0;
	IntStack* offset_stack;
	stackInit(offset_stack, 99);
	T_address_code T = malloc(sizeof(T_address_code));
	T_address_code S = malloc(sizeof(T_address_code));
	T_variable A1 = malloc(sizeof(T_variable));
	T_variable A2 = malloc(sizeof(T_variable));
	T_variable A3 = malloc(sizeof(T_variable));
	T_address tmp = malloc(sizeof(T_address));
	listFirst(L);

	while (1) {
		T = listGetItem(L);
		if (T == NULL) {
			return -1;
		}
		switch (T.operation) {

			//Instrukce pro aritmeticko operace
			case T_ADD:
				tmp = T.address1;
				if (tmp.isGlobal) {
					A1 = VStackGet(sVariableGlobal, tmp.offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, fram + tmp.offset;
				}
				tmp = T.address2;
				if (tmp.isGlobal) {
					A2 = VStackGet(sVariableGlobal, tmp.offset);
				}
				else {
					A2 = VStackGet(sVariableLocal, fram + tmp.offset);
				}
				tmp = T.result;
				if (tmp.isGlobal) {
					A3 = VStackGet(sVariableGlobal, tmp.offset);
				}
				else {
					A3 = VStackGet(sVariableLocal, fram + tmp.offset);
				}
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int + A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int + A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double + A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double + A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == STRING && A2.type == STRING) {
					String* tmp;
					tmp = A1.value.value_String;
					strLoad(A3.value.value_String, tmp->str);
					tmp = A2.value.value_String;
					strCat(A3.value.value_String, tmp->str);
				}
				else {
					return -1;
				}
				if (tmp.isGlobal) {
					VStackSet(sVariableGlobal, tmp.offset, A3);
				}
				else {
					VStackSet(sVariableLocal, frame + tmp.offset, A3);
				}
				break;

			case T_INC:
				tmp = T.address1;
				if (tmp.isGlobal) {
					A1 = VStackGet(sVariableGlobal, tmp.offset);
				}
				else {
					A1 = VStackGet(sVariableLocal, fram + tmp.offset;
				}
				if (A1.type == INT) {
					A1.value.value_int = A1.value.value_int + 1;
					ZeroFlag = !(A3.value.value_int == 0)
				}
				else {
					return -1;
				}
				if (tmp.isGlobal) {
					VStackSet(sVariableGlobal, tmp.offset, A1);
				}
				else {
					VStackSet(sVariableLocal, frame + tmp.offset, A1);
				}
				break;

			case T_SUB:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int - A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0)
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int - A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double - A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double - A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_DEC:
				A1 = VStackGet(vStack, frame + T.address1);
				if (A1.type == INT) {
					A1.value.value_int = A1.value.value_int - 1;
					ZeroFlag = !(A3.value.value_int == 0)
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.address1, A1);
				break;

			case T_MUL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int * A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0)
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int * A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double * A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double * A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_DIV:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int / A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0)
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.vaule_int / A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double / A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.valee_double = A1.value.value_double / A2.value.value_double;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			//Logicke operace
			case T_AND:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int && A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int && A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int && A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double && A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double && A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double && A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool && A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool && A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool && A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_OR:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int || A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int || A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int || A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double || A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double || A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double || A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool || A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool || A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool || A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_NOT:
				A1 = VStackGet(vStack, frame + T.address1);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					A3.value.value_int = !(A1.value.value_int);
					ZeroFlag = !(A3.value.value_int == 0);
				}
				else if (A1.type == DOUBLE) {
					A3.value.value_double = !(A1.value.value_double);
					ZeroFlag = !(A3.value.value_double == 0);
				}
				else if (A1.type == BOOLEAN) {
					A3.value.value_bool = !(A1.value.value_bool);
					ZeroFlag = !(A3.value.value_bool == 0);
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			//Porovnavaci operace
			case T_EQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int == A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int == A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int == A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double == A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double == A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double == A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool == A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool == A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool == A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_LEQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int <= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int <= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int <= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double <= A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double <= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double <= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool <= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool <= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool <= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_MEQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int >= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int >= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int >= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double >= A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double >= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double >= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool >= A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool >= A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool >= A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_NEQUAL:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int != A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int != A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int != A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double != A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double != A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double != A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool != A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool != A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool != A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_MORE:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int > A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int > A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int > A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double > A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double > A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double > A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool > A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool > A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool > A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			case T_LESS:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.result);
				if (A1.type == INT) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_int < A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_int < A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_int = A1.value.value_int < A2.value.value_bool;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == DOUBLE) {
					if (A2.type == INT) {
						A3.value.value_double = A1.value.value_double < A2.value.value_int;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_double < A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_double = A1.value.value_double < A2.value.value_bool;
						ZeroFlag = !(A3.value.value_double == 0);
					}
					else {
						return -1;
					}
				}
				else if (A1.type == BOOLEAN) {
					if (A2.type == INT) {
						A3.value.value_int = A1.value.value_bool < A2.value.value_int;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == DOUBLE) {
						A3.value.value_double = A1.value.value_bool < A2.value.value_double;
						ZeroFlag = !(A3.value.value_int == 0);
					}
					else if (A2.type == BOOLEAN) {
						A3.value.value_bool = A1.value.value_bool < A2.value.value_bool;
						ZeroFlag = !(A3.value.value_bool == 0);
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
				VStackSet(vStack, frame + T.result, A3);
				break;

			//Vestaveny funkce
			case T_IIN:
				int i = strReadInt();
				A1 = VStackGet(vStack, frame + T.result);
				A1.type = INT;
				A1.value.value_int = i;
				VStackSet(vStack, frame + T.result, A1);
				break;

			case T_DIN:
				double d = strReadDouble();
				A1 = VStackGet(vStack, frame + T.result);
				A1.type = DOUBLE;
				A1.value.value_double = d;
				VStackSet(vStack, frame + T.result, A1);
				break;

			case T_SIN:
				String* s = strReadString();
				A1 = VStackGet(vStack, frame + T.result);
				A1.type = STRING;
				A1.value.value_String = s;
				VStackSet(vStack, frame + T.result, A1);
				break;

			case T_OUT:
				A1 = VStackGet(vStack, frame + T.address1);
				if (A1.type == INT) {
					printf("%d", A1.value.value_int);
				}
				else if (A1.type == DOUBLE) {
					printf("%g", A1.value.value_double);
				}
				else if (A1.type == STRING) {
					strPrintStr(A1.value.value_String);
				}
				else {
					return -1;
				}
				break;

			case T_LENGTH:
				A1 = VStackGet(vStack, frame + T.address1);
				int i = strLength(A1.value.value_String);
				//ulozit i
				break;

			case T_SUBSTR:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				A3 = VStackGet(vStack, frame + T.address3);
				String s = strSubstr(A1.value.value_String, A2.value.value_int, A3.value.value_int);
				//ulozit s
				break;

			case T_COMPARE:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				int i = strCompare(A1.value.value_String, A2.value.value_String);
				//ulozit i
				break;

			case T_FIND:
				A1 = VStackGet(vStack, frame + T.address1);
				A2 = VStackGet(vStack, frame + T.address2);
				//find
				break;

			case T_SORT:
				A1 = VStackGet(vStack, frame + T.address1);
				//sort
				break;

			//Zasobnikove funkce
			case T_PUSH:
				VStackPush(vStack, T.address1);
				break;

			case T_POP:
				VStackPop(vStack);
				break;

			case T_MOV:
				//az budou zasobniky
				break;

			//Skokove instrukce pro if a cykly
			case T_LABEL:
				break;

			case T_JMPZD:
				if (!ZeroFlag) {
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

			//Instrukce pro funkcnost uzivatelskych funkci
			case T_FSTART:
				frame = T->address1;
				stackPush(offset_stack, T->address1);
				break;

			case T_FJMP:
				while (1) {
					listNext(L);
					S = listGetItem(L);
					if (S->operation == T_FLABEL && S->result == T->result) {
						break;
					}
					if (!(L->Active->next_item)) {
						return 1;
					}
				}
				A1 = VStackGet(vStack, frame + T->result);
				stackPop(offset_stack);
				VStackSet(vStack, frame, A1);
				if (stackEmpty(offset_stack)) {
					frame = 0;
				}
				else {
					frame = stackGet(offset_stack);
				}
				break;

			case T_FLABEL:
				break;

			case default:
				return -1;

		}
		if (L->Active->next_item == NULL) {
			free(T);
			free(S);
			free(A1);
			free(A2);
			free(A3);
			return 0;
		}
		listNext(L);
	}
}