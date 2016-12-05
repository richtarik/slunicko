///* File: instrList.c         */
///* Autori: Petr Mynarcik,    */
///*         Milos Molitoris   */
///* Login: xmynar05, xmolit00 */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.10.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "instrlist.h"
#include "struct.h"


//inicializace seznamu
void listInit (T_instr_list *L) {
	L->First = NULL;
	L->Active = NULL;
}

//vlozeni prvku do seznamu
void listInsert (T_instr_list *L, T_address_code *I) {
	T_item_list *newItem;
	newItem = memory_manager_malloc(sizeof(T_item_list));

	newItem->Instruction.operation = I->operation;
	newItem->Instruction.address1 = I->address1;
	newItem->Instruction.address2 = I->address2;
	newItem->Instruction.result = I->result;

	if (L->First == NULL) {
		newItem->next_item = L->First;
		L->First = newItem;
		L->Active = L->First;
	} else
	{
		L->Active->next_item = newItem;
		L->Active = L->Active->next_item;
		L->Active->next_item = NULL;
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
		L->Active = L->Active->next_item;
	}
}

//Vrati aktivni prvek seznamu
T_address_code* listGetItem(T_instr_list *L)
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
		L->First = L->First->next_item;
		memory_manager_free_one(tmp);
	}
}

//dalsi seznam

void fListInit(T_function_list *L) {
	L->First = NULL;
	L->Active = NULL;
}

void fListInsert(T_function_list *L, int id, int label, int iteration) {
	T_function_id_list *newItem;
	newItem = memory_manager_malloc(sizeof(T_function_id_list));

	newItem->Function.id = id;
	newItem->Function.label = label;
	newItem->Function.iteration = iteration;

	if (L->First == NULL) {
		newItem->next_item = L->First;
		L->First = newItem;
		L->Active = L->First;
	}
	else {
		L->Active->next_item = newItem;
		L->Active = L->Active->next_item;
		L->Active->next_item = NULL;
	}
}

void fListFirst(T_function_list *L) {
	L->Active = L->First;
}

void fListNext(T_function_list *L) {
	if (L->Active != NULL) {
		L->Active = L->Active->next_item;
	}
}

T_function_id* fListGetItem(T_function_list *L) {
	if (L->Active == NULL) {
		return NULL;
	}
	else {
		return &(L->Active->Function);
	}
}

int fListFind(T_function_list *L, int id) {
    if (L->First == NULL) {
        return -1;
    }
	T_function_id *tmp;
	tmp = memory_manager_malloc(sizeof(T_function_id));


	fListFirst(L);
	while (1) {
		tmp = fListGetItem(L);
		if (tmp->id == id) {
			return tmp->label;
		}
		if (L->Active->next_item == NULL) {
			return -1;
		}
		fListNext(L);
	}
}

void fListFree(T_function_list *L) {
	T_function_id_list *tmp;
	L->Active = NULL;
	while (L->First != NULL) {
		tmp = L->First;
		L->First = L->First->next_item;
		memory_manager_free_one(tmp);
	}
}
