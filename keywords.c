///* File: keywords.c          */
///* Autor: Milos Molitoris    */
///* Login: xmolit00           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 05.10.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */
#include "struct.h"
#include "keywords.h"
void f_findKeyW(T_token *token)
{
    char *keywords[]={"and","boolean", "break", "class", "continue", "do", "double", "else", "false", "for",
                      "if", "int", "return", "String", "static", "true", "void", "while"};

    token_type tokens[]={token_and, token_boolean, token_break, token_class, token_continue, token_do, token_double, token_else, token_false, token_for,
                         token_if, token_int, token_return, token_String_type, token_static, token_true, token_void, token_while};

    token->type = token_identifier;
    int i;
    for(i=0; i <= 17; i++)
    {
        if ( ( strcmp(keywords[i],token->value) )== 0)
        {
            token->type = tokens[i];
        }
    }
}


void f_find_key_w(T_token *token)
{
    token->type=token_identifier;

    switch (token->value[0])
    {
        case 'a':
            if (strcmp(token->value, "and") == 0)
            {
                token->type=token_and;
            }
            return;
        case 'b':
            if (strcmp(token->value, "boolean") == 0)
            {
                token->type=token_boolean;
            }
            else if (strcmp(token->value, "break") == 0)
            {
                token->type=token_break;
            }
            return;
        case 'c':
            if (strcmp(token->value, "continue") == 0)
            {
                token->type=token_continue;
            }
            else if (strcmp(token->value, "class") == 0)
            {
                token->type=token_class;
            }
            return;
        case 'd':
            if (strcmp(token->value, "do") == 0)
            {
                token->type=token_do;
            }
            else if (strcmp(token->value, "double") == 0)
            {
                token->type=token_double;
            }
            return;
        case 'e':
            if (strcmp(token->value, "else") == 0)
            {
                token->type=token_else;
            }
            return;
        case 'f':
            if (strcmp(token->value, "false") == 0)
            {
                token->type=token_false;
            }
            else if (strcmp(token->value, "for") == 0)
            {
                token->type=token_for;
            }
            return;
        case 'i':
            if (strcmp(token->value, "if") == 0)
            {
                token->type=token_if;
            }
            else if (strcmp(token->value, "int") == 0)
            {
                token->type=token_int;
            }
            return;
        case 'r':
            if (strcmp(token->value, "return") == 0)
            {
                token->type=token_return;
            }
            return;
        case 'S':
            if (strcmp(token->value, "String") == 0)
            {
                token->type=token_String_type;
            }
            return;
        case 's':
            if (strcmp(token->value, "static") == 0)
            {
                token->type=token_static;
            }
            return;
        case 't':
            if (strcmp(token->value, "true") == 0)
            {
                token->type=token_true;
            }
            return;
        case 'v':
            if (strcmp(token->value, "void") == 0)
            {
                token->type=token_void;
            }
            return;
        case 'w':
            if (strcmp(token->value, "while") == 0)
            {
                token->type=token_while;
            }
            return;
        default:
            return;
    }
}
