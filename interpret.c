#include "interpret.h"
#include "instrList.h"

int interpret(T_instr_list *L) {
	bool Flag = false;
	T_address_code T = malloc(sizeof(T_address_code));
	T_address_code S = malloc(sizeof(T_address_code));
	listFirst(L);

	while (1) {
		T = listGetItem(L);
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