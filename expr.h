#include "struct.h"
#include "stack.h"
#include <stdbool.h>

#define P_table_size 25


typedef enum
{
    operator_una,          // number 0 // "-a" // Unary minus
    operator_asg,          // number  // "=" // Assignment
    operator_add,          // number  // "+" // Addition
    operator_sub,          // number  // "-" // Subtraction
    operator_mul,          // number  // "*" // Multiplication
    operator_div,          // number  // "/" // Division
    operator_mod,          // number  // "%" // Modulo

    operator_inc,          // number  // "++" // Increment
    operator_dec,          // number  // "--" // Decrement

    operator_equ,          // number  // "==" // Equal
    operator_neq,          // number 10 // "!=" // Not equal
    operator_gre,          // number 11 // ">" // Greater
    operator_les,          // number 1 // "<" // Less
    operator_goe,          // number 1 // ">=" // Greater or equal
    operator_loe,          // number 1 // "<=" // Less or equal

    operator_or,           // number 15 // "||" // Or
    operator_and,          // number 1 // "&&" // And
    operator_not,          // number 1 // "!" // Not

    operator_pal,          // number 1 // "(" // Parenthesis left
    operator_par,          // number 1 // ")" // Parenthesis right

    operator_com,          // number 20 // "," // Comma

    operator_ID,           // number 21 //     // ID
    operator_fnc,          // number 2 //     // function
    operator_dlr,          // number 2 // "$" // Dollar
    operator_dot,           // number 25 // "."
    operator_NONTERM      // number 24 //     //
}operators;

typedef enum{
    symbol_equal =26 ,     // number 26 // "="
    symbol_less,           // number 27 // "<"
    symbol_greater,        // number 2 // ">"
    symbol_fault           // number 2 // "F"
} precedence_symbol;


int fn_expression(FILE *sourcefile, expr_in help);
operators fn_token_to_operators( expr_in help, int *count_paranthesis);
precedence_symbol fn_char_to_numsymbol(char c);

bool is_TOP_symbol_less_POP_and_PUSH_Nonterm(IntStack* stack);
void add_less_operator_to_stack(IntStack* stack);
//int Count_of_numbers_after_symbolless(IntStack* stack);
operators get_Top_operator_from_stack(IntStack* stack);
void check_rule(IntStack* stack,operators Rule_symbol);
//int Reduce(IntStack* stack);

/*
1.rule E->E=E ???????
2.ruĺe E->E+E
3.rule E->E-E
4.rule E->E*E
5.rule E->E/E
6.rule E->E%E
7.rule E->E++ ######
8.rule E->E-- ########
9.rule E->-E  ..................
10.rule E->E==E
11.rule E->E!=E
12.rule E->E>E
13.rule E->E<E
14.rule E->E>=E
15.rule E->E<=E
16.rule E->E||E
17.rule E->E&&E
18.rule E->!E
19.rule E->(E)
20.rule E->i
21.rule E->Fn
*/
