///* File: token.h             */
///* Autor: Milos Molitoris    */
///* Login: xmolit00           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 29.09.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>
#include <stdlib.h>

/* todo Tokens for: ???
ifj16

readInt
readDouble
readString

length
substr
compare
find
sort

*/

FILE* sourceFile;


/**
  zdroj: https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B
  */
typedef enum {

    //Arithmetic operators
    token_asg,          // number 0 // "=" // Assignment
    token_add,          // number 1 // "+" // Addition
    token_sub,          // number 2 // "-" // Subtraction
    token_mul,          // number 3 // "*" // Multiplication
    token_div,          // number 4 // "/" // Division
    token_mod,          // number 5 // "%" // Modulo

    token_inc,          // number 6 // "++" // Increment
    token_dec,          // number 7 // "--" // Decrement

    //Comparison operators/relational operators

    token_equ,          // number 8 // "==" // Equal
    token_neq,          // number 9 // "!=" // Not equal
    token_gre,          // number 10 // ">" // Greater
    token_les,          // number 11 // "<" // Less
    token_goe,          // number 12 // ">=" // Greater or equal
    token_loe,          // number 13 // "<=" // Less or equal

    //Logical operators //TODO state
    token_or,           // number 14 // "||" // Or
    token_and,          // number 15 // "&&" // And
    token_not,          // number 16 // "!" // Not

    //Bitwise operators

    //...TODO

    //other symbols ?? todo
    token_sem,          // number  // ";" // Semicolon
    token_com,          // number  // "," // Comma
    token_dot,          // number  // "." //

    token_pal,          // number  // "(" // Parenthesis left
    token_par,          // number  // ")" // Parenthesis right

    token_brl,          // number  // "[" // Bracket left
    token_brr,          // number  // "]" // Bracket right

    token_zlz,          // number  // "{" //
    token_zrz,          // number 25 // "}" //

    // KEYWORDS
    /*boolean, break, class, continue, do, double, else, false, for,
      if, int, return, String, static, true, void, while */

    token_break,        // number  //
    token_continue,     // number  //
    token_while,        // number  //
    token_for,          // number  //
    token_do,           // number 30 //

    //token_switch,
    //token_case,

    token_if,           // number  //
    token_else,         // number  //

    //token_function,
    token_return,       // number  //

    token_true,         // number  //
    token_false,        // number  //
    //token_null,

    token_class,
    token_void,

    token_double,
    token_int,
    token_String,

    token_boolean,
    token_static,
  //token_char,

    token_class_identifier,
    token_identifier,
    token_number_int,
    token_number_double,
    token_error,
    token_EOF,
    token_String_type,

	//enumy pouze pro generator-interpret
	token_label,
	token_jmpzd,
	token_jmpd,
	token_fstart,
	token_fjmp,
	token_flabel

} token_type;

typedef struct T_Token {
    token_type type;
    unsigned int valMaxsize;
    unsigned int valActsize;
    char *value;
} T_token;

T_token * token;

//Zasobnik pro ukladani promennych
typedef struct {
	T_variable* data;
	unsigned int max;
	int top;
} VariableStack;

//Enum typu promennych pro interpret
typedef enum {
	INT,
	DOUBLE,
	STRING,
	BOOLEAN
} value_type;

//Svaz promennych pro hodnoty ruznych datovych typu
union T_value {
	int value_int;
	double value_double;
	String *value_String;
	bool value_bool
} value_struct;

//Struktura pro ulozeni typu a hodnoty promenne
typedef struct {
	value_type type;
	union T_value value;
} T_variable;

//struktura pro ulozeni offsetu a rozliseni mezi lokalni a globalni promennou
typedef struct {
	int offset;
	bool isGlobal;
} T_address;

//struktura triadresnyho kodu
typedef struct {
    void *result;
    void *address1;
    void *address2;
    int operation;
} T_address_code;

//definice seznamu instrukci
typedef struct item_list {
    T_address_code Instrukcion;
    struct item_list *next_item;
} T_item_list;

//definice obousmerneho seznamu
typedef struct {
    struct item_list *First;
    struct item_list *Active;
} T_instr_list;

typedef enum
{
    in_if,
    in_while,
    in_assign,
    in_function,
    in_return,
}expr_in;

#endif // TOKENS_H
