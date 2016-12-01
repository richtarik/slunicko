///* File: lex.h               */
///* Autors: Milos Molitoris,  */
///* 		 Jiří Čechák	   */
///* Login: xmolit00,          */
///*        xcecha04           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 29.09.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#ifndef LEX_H
#define LEX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> // isdigit,isspace,...
#include <limits.h> // INT_MAX
#include <float.h> // DBL_MAX

#include "str.h"
#include "struct.h"
#include "keywords.h"
#include "error.h"
#include "memory_manager.h"

#define ALLOC_BLOCK 20
#define base_token_size 20

void Init_token(T_token *token);
void free_token(T_token *token);
void get_back_token(T_token *token);
void get_token(T_token *token,FILE* sourceFile);
void check_escape(FILE* sourceFile, T_token *actToken);
void check_String(FILE* sourceFile, T_token *actToken);
void f_Identifier(FILE* sourceFile, T_token *actToken);
void f_addChar (char symbol, T_token *token);
void comment_in_block(FILE* sourceFile);
void comment_in_line(FILE* sourceFile);
void Hex_to_Dec(T_token *token);
int hex_digit_to_dec_digit(char c);
double to_Dec(T_token *token, int soustava);

typedef enum
{
    state_Start,
    //state_Identifier,
    state_Number,
    state_Equal,
    state_Addition,
    state_Subtraction,
    state_Division_or_comment,
    state_Not,
    state_Greater,
    state_Less,
    state_Or,
    state_And,
    state_String,
    state_Double_dot,
    state_Double,
    state_Double_exp,
    state_Double_exp_1,
    state_Double_exp_sign,
    state_Double_sign_zero,
    state_Octal_prepare,
    state_Octal,
    state_Bin_first,
    state_Bin,
    state_Hex,
    state_Hex_continue,
    state_Hex_Pp,
    state_Hex_Pp_sign,
    state_Hex_Pp_exp,
    state_Hex_dot_first,
    state_Hex_dot,
    state_Hex_Pp_sign_zero

} lexikal_state;

#endif // TOKENS_H
