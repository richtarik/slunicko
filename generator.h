///* File: generator.h         */
///* Autor: Petr Mynarcik      */
///* Login: xmynar05           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 09.11.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "struct.h"
#include "instrList.h"
#include "interpret.h"
#include "error.h"
#include "stack.h"

int generator(T_instr_list *L, bool isRoot, VariableStack vStack);