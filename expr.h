#include "struct.h"
#include "stack.h"
#include "ial.h"
#include <stdbool.h>

#define P_table_size 25


typedef enum
{
    operator_una,          // number 0 // "-a" // Unary minus
//    operator_asg,          // number  // "=" // Assignment
    operator_add,          // number 1 // "+" // Addition
    operator_sub,          // number  // "-" // Subtraction
    operator_mul,          // number  // "*" // Multiplication
    operator_div,          // number  // "/" // Division
    operator_mod,          // number  // "%" // Modulo

    operator_Einc,          // number  // "++" // Postfix Increment
    operator_Edec,          // number  // "--" // Postfix Decrement

    operator_equ,          // number  // "==" // Equal
    operator_neq,          // number 9 // "!=" // Not equal
    operator_gre,          // number 10 // ">" // Greater
    operator_les,          // number 1 // "<" // Less
    operator_goe,          // number 1 // ">=" // Greater or equal
    operator_loe,          // number 1 // "<=" // Less or equal

    operator_or,           // number 14 // "||" // Or
    operator_and,          // number 15 // "&&" // And
    operator_not,          // number 1 // "!" // Not

    operator_pal,          // number 1 // "(" // Parenthesis left
    operator_par,          // number 1 // ")" // Parenthesis right

    operator_com,          // number 19 // "," // Comma

    operator_ID,           // number 20 //     // ID
    operator_fnc,          // number 2 //     // function
    operator_dlr,          // number 2 // "$" // Dollar

    operator_incE,          // number 23 // "++" // Prefix  Increment
    operator_decE,          // number  // "--" // Prefix  Decrement

    operator_NONTERM      // number 25 //     //
}operators;

typedef enum{
    symbol_equal =30 ,     // number 30 : 26 // "="
    symbol_less,           // number 31 : 27 // "<"
    symbol_greater,        // number 32 : // ">"
    symbol_fault           // number 33 : // "F"
} precedence_symbol;



int fn_expression(expr_in help,value_type * result_type, ht_item_func_ptr fnce, T_instr_list  ** instrList);
operators fn_token_to_operators( expr_in help, int *count_paranthesis, String * key_identifier, IntStack *stack);
precedence_symbol fn_char_to_numsymbol(char c);
void is_TOP_symbol_less_POP_and_PUSH_Nonterm(IntStack* stack);
void add_less_operator_to_stack(IntStack* stack);
operators get_Top_operator_from_stack(IntStack* stack);
void create_const(T_token* tokenik, T_variable* var);
void gen_code(int operation, void *address1, void *address2, void *result,T_instr_list *L_ins);
void set_adress_and_gen_code(token_type lool, VariableStack* Help_VSstack, T_instr_list  * MyinstrList);
void UNARY_gen_prefix(VariableStack* Help_VSstack,T_instr_list *instrList,int topik);

/*
02.ruĺe E->E+E
03.rule E->E-E
04.rule E->E*E
05.rule E->E/E
NO.rule E->E%E

07.rule E->++E ######
08.rule E->--E ########

07.rule E->E++ ######
08.rule E->E-- ########
09.rule E->-E  ..................

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
21.rule E->Fn()
22.rule E->Fn(E)
23.rule E->Fn(E,...)
*/
