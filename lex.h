///* File: lex.h              */
///* Autor: Milos Molitoris    */
///* Login: xmolit00           */
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

#include "token.h"

#define ALLOC_BLOCK 50

void get_token(T_token *token,FILE* filename);
void check_escape(FILE* sourceFile, T_token *actToken);
bool check_String(FILE* sourceFile, T_token *actToken);
bool f_Identifier(FILE* sourceFile, T_token *actToken);
bool f_addChar (char symbol, T_token *token);
bool comment_in_block(FILE* filename);
bool comment_in_line(FILE* filename);

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
    state_Double_exp_sign,

} lexikal_state;

#endif // TOKENS_H
