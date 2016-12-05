///* File: instlist.h          */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.10.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "struct.h"
#include "memory_manager.h"

void listInit(T_instr_list *L);
void listInsert(T_instr_list *L, T_address_code *I);
void listFirst(T_instr_list *L);
void listNext(T_instr_list *L);
T_address_code* listGetItem(T_instr_list *L);
void listFree(T_instr_list *L);

void fListInit(T_function_list *L);
void fListInsert(T_function_list *L, int id, int label, int iteration);
void fListFirst(T_function_list *L);
void fListNext(T_function_list *L);
T_function_id* fListGetItem(T_function_list *L);
int fListFind(T_function_list *L, int id);
void fListFree(T_function_list *L);