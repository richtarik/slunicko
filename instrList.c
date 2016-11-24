///* File: instrList.c         */
///* Autori: Petr Mynarcik,    */
///*         Milos Molitoris   */
///* Login: xmynar05, xmolit00 */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.10.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "instrList.h"
#include "struct.h"

void gen_code(int operation, void *address1, void *address2, void *result)
{
    T_address_code *ins = memory_manager_malloc(sizeof(T_address_code));
    ins->operation = operation;
    ins->address1 = address1;
    ins->address2 = address2;
    ins->result = result;

    listInsert(L_ins, ins);
}

void gen_expr_code()
{

}

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
