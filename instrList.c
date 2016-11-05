#include "instrList.h"
#include "struct.h"

//inicializace seznamu
void listInit (T_intsr_list *L) {
	L->First = NULL;
	L->Active = NULL;
}

//vlozeni prvku do seznamu
void listInsert (T_intsr_list *L, T_address_code *I) {
	T_item_list *newItem;
	newItem = malloc(sizeof(T_item_list));

	newItem->Instruction.operation = I->operation;
	newItem->Instruction.address1 = I->address1;
	newItem->Instruction.address2 = I->address2;
	newItem->Instruction.result = I->result;

	if (L->First == NULL) {
		newItem->nextItem = L->First;
		L->First = newItem;
		L->Active = L->First;
	} else 
	{
		L->Active->nextItem = newItem;
		L->Active = L->Active->nextItem;
	}
}

//Nastaveni ukazatele aktivity na prvni prvek
void listFirst(T_instr_list *L)
{
	L->Active = L->First;
}

//Posunuti ukazatele aktivity o jeden prvek
void listNext(T_instr_list *L)
{
	if (L->Active != NULL) {
		L->Active = L->Active->nextItem;
	}
}

//Vrati aktivni prvek seznamu
T_address_code *listGetItem(T_instr_list *L)
{
	if (L->Active == NULL)
	{
		return NULL;
	}
	else {
		return &(L->Active->Instruction);
	}
}

//Vymazani celeho seznamu a uvolneni pameti
void listFree(T_instr_list *L) {
	T_item_list *tmp;
	L->Active = NULL;
	while (L->First != NULL) {
		tmp = L->First;
		L->First = L->First->nextItem;
		free(tmp);
	}
}