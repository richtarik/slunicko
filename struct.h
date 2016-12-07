///* File: struct.h            */
///* Autor:                    */
///* Login:                    */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 29.09.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


FILE* sourceFile;
bool firstScan; //prepinac ci som v prvom alebo druhom zostupe
int actFunctionOffset;


/**
  zdroj: https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B
  */
typedef enum {

    token_asg,          // number 0 // "=" // Assignment
    token_add,          // number 1 // "+" // Addition
    token_sub,          // number 2 // "-" // Subtraction
    token_mul,          // number 3 // "*" // Multiplication
    token_div,          // number 4 // "/" // Division
///NO USE MODULO (for future)
    token_mod,          // number 5 // "%" // Modulo
///NO USE MODULO (for future)
    token_inc,          // number 6 // "++" // Increment ///UNARY
    token_dec,          // number 7 // "--" // Decrement ///UNARY

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

    //Other Symbols
    token_sem,          // number 17 // ";" // Semicolon
    token_com,          // number 18 // "," // Comma
    token_dot,          // number 19 // "." //

    token_pal,          // number 20 // "(" // Parenthesis left
    token_par,          // number 21 // ")" // Parenthesis right

    token_brl,          // number 22 // "[" // Bracket left
    token_brr,          // number 23 // "]" // Bracket right

    token_zlz,          // number 24 // "{" //
    token_zrz,          // number 25 // "}" //

    // KEYWORDS
    /*boolean, break, class, continue, do, double, else, false, for,
      if, int, return, String, static, true, void, while */

    token_break,        // number 26 // TODO
    token_continue,     // number 27 // TODO
    token_while,        // number 28 //
    token_for,          // number 29 // TODO
    token_do,           // number 30 // TODO

    token_if,           // number 31 //
    token_else,         // number 32 //

    token_return,       // number 33 //

    token_true,         // number 34 //
    token_false,        // number 35 //

    token_class,        // number 36 //
    token_void,         // number 37 //

    token_double,       // number 38 //
    token_int,          // number 39 //
    token_String_type,  // number 40 //

    token_boolean,      // number 41 //
    token_static,       // number 42 //

    token_class_identifier,// number 43 // TODO NO USE HISTRORY
    token_identifier,      // number 44 //
    token_number_int,      // number 45 //
    token_number_double,   // number 46 //
    token_error,           // number 47 //
    token_EOF,             // number 48 //
    token_String,          // number 49 //

	//enumy pouze pro generator-interpret
	token_label,
	token_jmpzd,
	token_jmpd,
	token_jmpu,
	token_fstart,
	token_fjmp,             //55
	token_flabel,
	token_push,
	token_pull,
	token_mov,
	token_func,             // number 60 //
	//vestavene funkce
	token_iin,              // number 61 // ifj16.readInt
	token_din,              // number 62 // ifj16.readDouble
	token_sin,              // number 63 // ifj16.readString
	token_out,              // number 64 // ifj16.print
	token_length,           // number 65 // ifj16.length
	token_substr,           // number 66 // ifj16.substr
	token_compare,          // number 67 // ifj16.compare
	token_find,             // number 68 // ifj16.find
	token_sort,             // number 70 // ifj16.sort
	token_unm               // number 71 // unarne minus - UNARY

} token_type;

typedef struct T_Token {
    token_type type;
    unsigned int valMaxsize;
    unsigned int valActsize;
    char *value;
} T_token;

T_token * token;

//Enum typu promennych pro interpret
typedef enum {
    DOUBLE=38,
    INT,
    STRING,
    BOOLEAN,
} value_type;

typedef struct String str;
//Svaz promennych pro hodnoty ruznych datovych typu
    union T_value {
	int value_int;
	double value_double;
	str* value_String;
	bool value_bool;
} value_struct;

//Struktura pro ulozeni typu a hodnoty promenne
typedef struct {
	value_type type;
	union T_value value;
} T_variable;

//Zasobnik pro ukladani promennych
typedef struct {
	T_variable* data;
	unsigned int max;
	int top;
} VariableStack;

///help stack for expr + sematika
typedef enum {
	ADRESS_L,
	CONSTANT,
	ADRESS_G
} value_type_Stack;


typedef struct {
    value_type type;
    int offset;
    value_type_Stack GlobLokCons;
}T_Exp_variable;

typedef struct {
	T_Exp_variable* data;
	unsigned int max;
	int top;
} ExpStack;

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
    T_address_code Instruction;
    struct item_list *next_item;
} T_item_list;

//definice jednosmerneho seznamu
typedef struct {
    struct item_list *First;
    struct item_list *Active;
} T_instr_list;


//seznamy pro funkce v interpretu
typedef struct {
	int id;
	int label;
	int iteration;
} T_function_id;

typedef struct function_id_list {
	T_function_id Function;
	struct function_id_list *next_item;
} T_function_id_list;

typedef struct {
	struct function_id_list *First;
	struct function_id_list *Active;
} T_function_list;

T_instr_list* L_ins;

typedef enum
{
    in_if,
    in_while,
    in_assign_first,
    in_assign_second,
    in_function,
    in_return,
    in_length_fun,
    in_substr_fun,
    in_find_fun,
    in_sort_fun,
    in_compare_fun,
}expr_in;

VariableStack * sVariableGlobal;
VariableStack * sVariableLocal;
#endif // TOKENS_H
