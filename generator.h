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
#include "memory_manager.h"
#include "stack.h"

//Predefinovani aritmetickych operac
#define T_ADD token_add
#define T_INC token_inc
#define T_SUB token_sub
#define T_DEC token_dec
#define T_MUL token_mul
#define T_DIV token_div
//Predefinovani logickych operaci
#define T_AND token_and
#define T_OR token_or
#define T_NOT token_not
//Predefinovani porovnavacich operaci
#define T_EQUAL token_equ
#define T_LEQUAL token_loe
#define T_MEQUAL token_goe
#define T_NEQUAL token_neq
#define T_MORE token_gre
#define T_LESS token_les
//Predefinovai operaci pro vstup a vystup
#define T_IN 100
#define T_OUT 101
//Predefinovani cyklu, funkci, ...
#define T_IF token_if
#define T_WHILE token_while
#define T_FUNC 99
#define T_RETURN token_return
//Definovani skoku
#define T_LABEL token_label
#define T_JMPZD token_jmpzd
#define T_JMPD token_jmpd
#define T_FSTART token_fstart
#define T_FJMP token_fjmp
#define T_FLABEL token_flabel

int generator(T_instr_list *L, bool isRoot);
