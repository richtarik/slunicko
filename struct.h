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
#endif // TOKENS_H


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

    //other symbols
    token_sem,          // number 17 // ";" // Semicolon
    token_col,          // number 18 // "," // Colon
    token_dot,          // number 19 // "." //

    token_pal,          // number 20 // "(" // Parenthesis left
    token_par,          // number 21 // ")" // Parenthesis right

    token_brl,          // number 22 // "[" // Bracket left
    token_brr,          // number 23 // "]" // Bracket right

    token_zlz,          // number 24 // "{" //
    token_zrz,          // number 25 // "}" //

    // KEYWORDS
    /* boolean, break, class, continue, do, double, else, false, */
    /* for, if, int, return, String, static, true, void, while   */

    token_break,        // number 26 //
    token_continue,     // number 27 //
    token_while,        // number 28 //
    token_for,          // number 29 //
    token_do,           // number 30 //

    //token_switch,
    //token_case,

    token_if,           // number 31 //
    token_else,         // number 32 //

    token_return,       // number 33 //

    token_true,         // number 34 //
    token_false,        // number 35 //
    //token_null,

    token_class,        // number 36 //
    token_void,         // number 37 //

    token_double,       // number 38 //
    token_int,          // number 39 //
    token_String,

    token_boolean,      // number 40 //
    token_static,       // number 41 //
    //token_char,

    // ID
    token_class_identifier, // number 42 //
    token_identifier,       // number 43 //
    token_number_int,       // number 44 //
    token_number_double,    // number 45 //

    // ERROR, EOF
    token_error,            // number 46 //
    token_EOF,              // number 47 //
    token_String_type

} token_type;


typedef struct T_Token {
    token_type type;
    unsigned int valMaxsize;
    unsigned int valActsize;
    char *value;
} T_token;

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
