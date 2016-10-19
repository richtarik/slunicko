#include "struct.h"

void listInit(T_instr_list *L);
void listInsert(T_instr_list *L, T_address_code *I);
void listFirst(T_instr_list *L);
void listNext(T_instr_list *L);
T_address_code listGetItem(T_instr_list *L);
void listFree(T_instr_list *L);