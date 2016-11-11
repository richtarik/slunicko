#include "expr.h"
//#include "struct.h"
//#include "lex.h"

const char Precedence_table[P_table_size][P_table_size]={
// todo unary- "-a" assign == $
//          #                                  #      #    #
//      "unary-"  "="   "+"   "-"   "*"   "/"   "%"   "++"  "--"  "=="  "!="  ">"   "<"   ">="  "<="  "||"  "&&"  "!"   "("   ")"   "ID"  "Fn"  "$"
/*unry-*/ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' }, //unr-//
/*  =  */ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' }, // =  //
/*  +  */ { '#' , '#' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // +  //
/*  -  */ { '#' , '#' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // -  //
/*  *  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '<' , '<' , '>' }, // *  //
/*  /  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '<' , '<' , '>' }, // /  //
/*  %  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '<' , '<' , '>' }, // %  //
/* ++  */ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' }, // ++ // --
/* --  */ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' }, // -- // --
/* ==  */ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' }, // == // --
/* !=  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // != //
/*  >  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // >  //
/*  <  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // <  //
/* >=  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // >= //
/* <=  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // <= //
/* ||  */ { '#' , '#' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '<' , '<' , '>' }, // || //
/* &&  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '<' , '<' , '>' }, // && //
/*  !  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '<' , '>' , '<' , '<' , '>' }, // !  //
/*  (  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , ' ' }, // (  //
/*  )  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , 'F' , 'F' , '>' }, // )  //
/* ID  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , 'F' , 'F' , '>' }, // ID //
/* Fn  */ { '#' , '#' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , '=' , 'F' , 'F' , 'F' , 'F' }, // Fn //
/*  $  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , 'F' , '<' , '<' , 'F' }, // $  //
};

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
operators fn_token_to_operators(T_token * token)
{
    switch(token->type)
    {

        case token_asg:
            return operator_asg;          // number 0 // "=" // Assignment

        case token_add:
            return operator_add;          // number 1 // "+" // Addition

        case token_sub:
            return operator_sub;          // number 2 // "-" // Subtraction

        case token_mul:
            return operator_mul;          // number 3 // "*" // Multiplication

        case token_div:
            return operator_div;          // number 4 // "/" // Division

        case token_mod:
            return operator_mod;          // number 5 // "%" // Modulo

        case token_inc:
            return operator_inc;          // number 6 // "++" // Increment

        case token_dec:
            return operator_dec;          // number 7 // "--" // Decrement

        case token_equ:
            return operator_equ;          // number 8 // "==" // Equal

        case token_neq:
            return operator_neq;          // number 9 // "!=" // Not equal

        case token_gre:
            return operator_gre;          // number 10 // ">" // Greater

        case token_les:
            return operator_les;          // number 11 // "<" // Less

        case token_goe:
            return operator_goe;          // number 12 // ">=" // Greater or equal

        case token_loe:
            return operator_loe;          // number 13 // "<=" // Less or equal

        case token_or:
            return operator_or;           // number 14 // "||" // Or

        case token_and:
            return operator_and;          // number 15 // "&&" // And

        case token_not:
            return operator_not;          // number 16 // "!" // Not


        case token_pal:
            return operator_pal;          // number 17 // "(" // Parenthesis left

        case token_par:
            return operator_par;          // number 18 // ")" // Parenthesis right

        case token_identifier:
            get_token(token,sourceFile);  //TODO
            if( token->type == token_pal )
            {
                get_back_token(token);
                return operator_fnc;
            }
            else
            {
                get_back_token(token);
                return operator_ID;
            }

        case token_String:
        case token_true:
        case token_false:
        case token_number_int:
        case token_number_double:
            return operator_ID;            // number 19 // ID

        case token_sem:
        case token_col:
            return operator_dlr;          // number 21 // "$" // Dollar
        default:
            return operator_NONTERM;
    }

}

precedence_symbol fn_char_to_numsymbol(char c)
{
    switch(c)
    {
        case'<':
            return symbol_less;
        case'>':
            return symbol_greater;
        case'=':
            return symbol_equal;
        case'F':
            return symbol_fault;
//      default:
//          return
    }
}

#define stack_size 20

int fn_expresion(FILE* sourceFile)
{

    char c;
    precedence_symbol symbol_from_table;
    operators My_operator;
    IntStack zasobnik;
    T_token token;
    int topik;
    get_token( &token, sourceFile );

    stackInit(&zasobnik,stack_size);
    stackPush(&zasobnik,operator_dlr);
//DBGGGGGGG
    print_stack_data(&zasobnik);

    do
    {
        My_operator= fn_token_to_operators( &token );
        if(My_operator == operator_NONTERM)
            return 0; // TODO break free stack

        c= Precedence_table[ get_Top_operator_from_stack( &zasobnik ) ][ My_operator ];
        symbol_from_table= fn_char_to_numsymbol(c);

        switch(symbol_from_table)
        {
            case symbol_equal:
        // DBGG DELETE
                printf("symbol_equ\n");
                stackPush(&zasobnik,My_operator);
                print_stack_data(&zasobnik);
                stackPop(&zasobnik);
        // DBG DELETE

                stackPop(&zasobnik);
                stackPop(&zasobnik);
                stackPop(&zasobnik);
                stackPush(&zasobnik,operator_NONTERM);

                get_token( &token, sourceFile );
                break;
            case symbol_less:
                add_less_operator_to_stack(&zasobnik);
                stackPush(&zasobnik,My_operator);
                get_token( &token, sourceFile );
                break;
            case symbol_greater:
                stackTop(&zasobnik,&topik);
                //RULE 20 E-> i
                if(topik == operator_ID)
                {
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                    if( topik == symbol_less)
                    {
                        stackPop(&zasobnik);
                        stackPush(&zasobnik,operator_NONTERM);

                    }
                    else
                    {
                        printf("ERROR -1\n");
                        stackPush(&zasobnik,operator_dlr);
                        break;
                    }
                }
                else if( topik == operator_par )
                {
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                }
                else if( topik == operator_NONTERM)
                {
                    // check_rule(); -> REDUCE
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                    switch(topik)
                    {
                      case operator_add:
                          printf("add\n");
                          break;
                      case operator_sub:
                          printf("sub\n");
                          break;
                      case operator_mul:
                          printf("mul\n");
                          break;
                      case operator_div:
                          printf("div\n");
                          break;
                      case operator_mod:
                          printf("mod\n");
                          break;

                          // ++
                          // --

                      case operator_equ:
                          printf("equ\n");
                          break;
                      case operator_neq:
                          printf("neq\n");
                          break;
                      case operator_gre:
                          printf("gre\n");
                          break;
                      case operator_les:
                          printf("les\n");
                          break;
                      case operator_goe:
                          printf("goe\n");
                          break;
                      case operator_loe:
                          printf("loe\n");
                          break;

                      case operator_or:
                          printf("or\n");
                          break;
                      case operator_and:
                          printf("goe\n");
                          break;
                      case operator_not:
                          printf("loe\n");
                          break;
                    }
                    stackPop(&zasobnik);
                    check_rule(&zasobnik,topik);

                }
                else
                {
                    //TODO ERROR free stack return err code
                    printf("ERROR 0\n");
                    stackPush(&zasobnik,operator_dlr);
                    break;
                }
                break;



          case symbol_fault:
                printf("end\n");
                print_stack_data(&zasobnik);
                return 0; // TODO break free stack
       }
        /*Stack_Top(stack,&topik)
        //20.rule E->i
        if( get_Top_operator_from_stack( zasobnik ) == operator_dlr && topik == operator_NONTERM )
        {                       //TODO je to less symbol ? ak to neni less ->koniec
          Stack_push(&zasobnik,symbol_from_table);
          Stack_push(&zasobnik,My_operator);
        }

        check_rule(&zasobnik);
        */

        print_stack_data(&zasobnik);

    }while( stackEmpty(&zasobnik) || topik!=operator_dlr );

  return 1;
}


void check_rule(IntStack* stack,operators Rule_symbol)
{
    int topik;
    stackTop(stack,&topik);
    if(topik == operator_NONTERM)
    {
        stackPop(stack);
        stackTop(stack,&topik);
        if(topik == symbol_less)
        {
            stackPop(stack);
            stackPush(stack,operator_NONTERM);
        }
        else
            printf("ERROR 1- check rule\n");// TODO
    }
    else
    {
        printf("ERROR 2- check rule\n");// TODO
    }
    //stackPop(&zasobnik);


}

operators get_Top_operator_from_stack(IntStack* stack)
{
    int pom;
    stackTop(stack,&pom);
    if ( pom != operator_NONTERM)
    {
        return pom;
    }
    else
    {
        stackPop(stack);
        stackTop(stack,&pom);
        stackPush(stack,operator_NONTERM);
        return pom;
    }

}

void add_less_operator_to_stack(IntStack* stack)
{
    int pom;
    stackTop(stack,&pom);
    if ( pom != operator_NONTERM)
    {
        stackPush(stack,symbol_less);
    }
    else
    {
        stackPop(stack);
        stackPush(stack,symbol_less);
        stackPush(stack,operator_NONTERM);
        return pom;
    }
}
