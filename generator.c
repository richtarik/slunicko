#include "generator.h"
#include "instrList.h"

static unsigned int labelTemp = 1;
static unsigned int functionTemp = 1;
static unsigned int *label = &labelTemp;
static unsigned int *functionLabel = &functionTemp;
static T_instr_list *iList;

int generator(T_instr_list *L, bool isRoot) {
	if (!L) {
		return 1;
	}

	if (isRoot) {
		iList = malloc(sizeof(T_instr_list));
		listInit(iList);
	}

	
	T_address_code *T;
	T_address_code *S;
	T_address_code *R;
	T = malloc(sizeof(T_address_code));
	S = malloc(sizeof(T_address_code));
	R = malloc(sizeof(T_address_code));
	listFirst(L);
	if (L->First == NULL || L->Active == NULL) {
		return 1;
	}
	while (1) {
		T = listGetItem(L);
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
				generator(T->address1, false);
				S->operation = T_JMPZD;
				S->result = label;
				labelTemp++;
				listInsert(iList, S);
				generator(T->address2, false);
				if (T->result) {
					R->operation = T_JMPD;
					R->result = label;
					labelTemp++;
					listInsert(iList, R);
				}
				S->operation = T_LABEL;
				listInsert(iList, S);
				if (T->result) {
					generator(T->result, false);
					R->operation = T_LABEL;
					listInsert(iList, R);
				}
				break;

			case T_WHILE:
				S->operation = T_LABEL;
				S->result = label;
				labelTemp++;
				listInsert(iList, S);
				generator(T->address1, false);
				R->operation = T_JMPZD;
				R->result = label;
				labelTemp++;
				listInsert(iList, R);
				generator(T->address2, false);
				S->operation = T_JMPU;
				listInsert(iList, S);
				R->operation = T_LABEL;
				listInsert(iList, R);
				break;

			//funkce
			
			case T_FUNC:

			case T_RETURN:

			case DEFAULT:
				return 1;
		}

		if (L->Active->nextItem == NULL) {
			break;
		}
	}
	return 0;
}