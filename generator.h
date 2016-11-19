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
#define T_IIN token_iin
#define T_DIN token_din
#define T_SIN token_sin
#define T_OUT token_out
//Vestavene funkce
#define T_LENGTH token_length
#define T_SUBSTR token_substr
#define T_COMPARE token_compare
#define T_FIND token_find
#define T_SORT token_sort
//Predefinovani cyklu, funkci, ...
#define T_IF token_if
#define T_WHILE token_while
#define T_FUNC token_func
#define T_RETURN token_return
//Definovani skoku
#define T_LABEL token_label
#define T_JMPZD token_jmpzd
#define T_JMPD token_jmpd
#define T_FSTART token_fstart
#define T_FJMP token_fjmp
#define T_FLABEL token_flabel
#define T_PUSH token_push
#define T_POP token_pull
#define T_MOV token_mov

int generator(T_instr_list *L, bool isRoot);
