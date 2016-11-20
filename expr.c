#include "expr.h"
#include "struct.h"
#include "lex.h"

const char Precedence_table[P_table_size][P_table_size]={
// todo unary- "-a" assign == $
//          #                                  #      #    #
//      "unary-"  "="   "+"   "-"   "*"   "/"   "%"   "++"  "--"  "=="  "!="  ">"   "<"   ">="  "<="  "||"  "&&"  "!"   "("   ")"   ","   "ID"  "Fn"  "$"   "."
/*unry-*/ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , 'F' , '#' , '#' , '#' , '#' }, //unr-//
/*  =  */ { '#' , 'F' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '>' , '#' , '#' , '#' , '<' }, // =  //
/*  +  */ { '#' , '#' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // +  //
/*  -  */ { '#' , '#' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // -  //
/*  *  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // *  //
/*  /  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // /  //
/*  %  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // %  //
/* ++  */ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' , '#' , '<' }, // ++ // --
/* --  */ { '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '>' , '#' , '#' , '#' , '#' , '<' }, // -- // --
/* ==  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // == // --
/* !=  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // != //
/*  >  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // >  //
/*  <  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // <  //
/* >=  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // >= //
/* <=  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // <= //
/* ||  */ { '#' , '#' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // || //
/* &&  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // && //
/*  !  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '<' , '>' , '>' , '<' , '<' , '>' , '<' }, // !  //
/*  (  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '=' , '<' , '<' , '>' , '<' }, // (  //
/*  )  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , '>' , 'F' , 'F' , '>' , '<' }, // )  //
/*  ,  */ { '#' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '=' , '<' , '<' , 'F' , '<' }, // ,  //
/* ID  */ { '#' , '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , '>' , 'F' , 'F' , '>' , '>' }, // ID //
/* Fn  */ { '#' , '#' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , '=' , 'F' , '>' , 'F' , 'F' , 'F' , 'F' }, // Fn //
/*  $  */ { '#' , '#' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , 'F' , 'F' , '<' , '<' , 'F' , '<' }, // $  //
/*  .  */ { '#' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , '>' , '<' , '<' , '>' , 'F' }, // .  //
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
operators fn_token_to_operators(expr_in help, int *count_paranthesis)
{
    switch(token->type)
    {

        case token_asg:
            return operator_asg;          // number 1 // "=" // Assignment

        case token_add:
            return operator_add;          // number 2 // "+" // Addition

        case token_sub:
            return operator_sub;          // number 3 // "-" // Subtraction

        case token_mul:
            return operator_mul;          // number 4 // "*" // Multiplication

        case token_div:
            return operator_div;          // number 5 // "/" // Division

        case token_mod:
            return operator_mod;          // number 6 // "%" // Modulo

        case token_inc:
            return operator_inc;          // number 7 // "++" // Increment

        case token_dec:
            return operator_dec;          // number 8 // "--" // Decrement

        case token_equ:
            return operator_equ;          // number 9 // "==" // Equal

        case token_neq:
            return operator_neq;          // number 10 // "!=" // Not equal

        case token_gre:
            return operator_gre;          // number 11 // ">" // Greater

        case token_les:
            return operator_les;          // number 12 // "<" // Less

        case token_goe:
            return operator_goe;          // number 13 // ">=" // Greater or equal

        case token_loe:
            return operator_loe;          // number 14 // "<=" // Less or equal

        case token_or:
            return operator_or;           // number 15 // "||" // Or

        case token_and:
            return operator_and;          // number 16 // "&&" // And

        case token_not:
            return operator_not;          // number 17 // "!" // Not

        case token_pal:
            *count_paranthesis= *count_paranthesis +1;
            return operator_pal;          // number 18 // "(" // Parenthesis left

        case token_par:
            *count_paranthesis= *count_paranthesis -1;
            if( *count_paranthesis == 0 && help != in_assign)
                return operator_dlr;          // number 19 // ")" // Parenthesis right
            else
                return operator_par;

        case token_com:
            return operator_com;          // number 20 // "," // Comma

        case token_dot:
            return operator_dot;          // number 25 // "."

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
            return operator_ID;            // number  // ID

        case token_zlz: // {
        case token_sem: // ;
            return operator_dlr;          // number  // "$" // Dollar
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
        default:
            return symbol_fault;
    }
}

#define stack_size 20

int fn_expression(FILE* sourceFile, expr_in help)
{
    char c;
    precedence_symbol symbol_from_table;
    operators My_operator;
    IntStack zasobnik;
    int topik;

    int count_paranthesis;
    if( help == in_assign)
    {
        count_paranthesis=0;
    }
    else
        count_paranthesis=1;

    get_token( token, sourceFile );
    if( help == in_return && token->type == token_sem )
    {
        stackDelete_and_free(&zasobnik);
        //print_stack_data(&zasobnik);
        get_back_token(token);
        return 1;
    }

    My_operator= fn_token_to_operators( help, &count_paranthesis );

    stackInit(&zasobnik,stack_size);
    stackPush(&zasobnik,operator_dlr);
//DBGGGGGGG
    //print_stack_data(&zasobnik);
//DBGGGGGGG
    if ( (help == in_function) && (token->type == token_pal) )
    {
        stackPush(&zasobnik, symbol_less);
        stackPush(&zasobnik, operator_fnc);
        //DBGG
        //printf("Expresion_in_fnc");
        //print_stack_data(&zasobnik);
        //DBG

    }


    do
    {

        if(My_operator == operator_NONTERM)
        {
            stackDelete_and_free(&zasobnik);
            printf("delete and free 000\n");
           // //print_stack_data(&zasobnik);
            return 0; // TODO break free stack

        }
        c= Precedence_table[ get_Top_operator_from_stack( &zasobnik ) ][ My_operator ];
        symbol_from_table= fn_char_to_numsymbol(c);

        switch(symbol_from_table)
        {
            case symbol_equal:
        // DBGG DELETE
             //   printf("symbol_equ\n");
                stackPush(&zasobnik,My_operator);
                //print_stack_data(&zasobnik);
                //stackPop(&zasobnik);
        // DBG DELETE
                get_token( token, sourceFile );
                My_operator= fn_token_to_operators( help , &count_paranthesis );
                break;
            case symbol_less:
                add_less_operator_to_stack(&zasobnik);
                stackPush(&zasobnik,My_operator);
                get_token( token, sourceFile );
                My_operator= fn_token_to_operators( help , &count_paranthesis );
                break;
            case symbol_greater:
                stackTop(&zasobnik,&topik);
                //RULE 20 E-> i
                if(topik == operator_ID)
                {
                    stackPop(&zasobnik);
                    if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik) )
                        ;
                    else
                    {
                        printf("ERROR -1\n");
                        stackDelete_and_free(&zasobnik);
                        printf("delete and free\n");
                        //print_stack_data(&zasobnik);
                        return 0; // TODO break free stack
                    }
                }
                // RULE E-> fn( )
                // RULE E-> fn( E )
                // RULE E-> ( E )
                // RULE E-> fn( E, E, ...)
                else if( topik == operator_par )
                {
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                    // E -> fn( )
                    if( topik == operator_pal)
                    {
                        stackPop(&zasobnik);
                        stackTop(&zasobnik,&topik);
                        if( topik == operator_fnc )
                        {
                            stackPop(&zasobnik);
                            stackTop(&zasobnik,&topik);
                            if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik) )
                                ;
                            else
                            {
                                stackDelete_and_free(&zasobnik);
                                printf("delete and free 001\n");
                                //print_stack_data(&zasobnik);
                                return 0; // TODO break free stack
                            }
                        }
                        else
                        {
                            stackDelete_and_free(&zasobnik);
                            printf("delete and free 002\n");
            //                //print_stack_data(&zasobnik);
                            return 0; // TODO break free stack
                        }
                    }
                    // RULE E-> fn( E )
                    // RULE E-> ( E )
                    // RULE E-> fn( E, E, ...)
                    else if( topik == operator_NONTERM)
                    {
                        stackPop(&zasobnik);
                        stackTop(&zasobnik,&topik);
                        // RULE E-> fn( E )
                        // RULE E-> ( E )
                        if( topik == operator_pal)
                        {

                            stackPop(&zasobnik);
                            stackTop(&zasobnik,&topik);
                            // RULE E-> fn( E )
                            if( topik == operator_fnc )
                            {
                            //    printf("funkcia s jednym parametrom \n");
                                stackPop(&zasobnik);
                                if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik) )
                                    ;
                                else
                                {
                                    printf("error 251987\n");;//TODOooooooooooo error
                                    stackDelete_and_free(&zasobnik);
                                    printf("delete and free\n");
                          //          //print_stack_data(&zasobnik);
                                    return 0; // TODO break free stack
                                }
                            }
                            // RULE E-> ( E )
                            else if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik) )
                                printf("parameter v zatvorke \n");
                            else
                            {
                                printf("error 1987\n");;//TODOooooooooooo error
                                stackDelete_and_free(&zasobnik);
                                printf("delete and free\n");
                          //      //print_stack_data(&zasobnik);
                                return 0; // TODO break free stack
                            }
                        }
                        // RULE E-> fn( E, E, ...)
                        else if( topik == operator_com)
                        {
                         //   printf("funkcia s viacerimi parametrami \n");
                            bool check_E_before= true;
                            unsigned count_parameters= 1;

                            stackPop(&zasobnik);
                            stackTop(&zasobnik,&topik);
                          //  printf("pred cyklom pocet param: %d\n", count_parameters);
                  //          //print_stack_data(&zasobnik);

                            while ( topik != operator_pal )
                            {
                                if ( ( topik == operator_NONTERM ) && check_E_before )
                                {
                                    check_E_before= false;
                                    count_parameters++;
                                    stackPop(&zasobnik);
                                }
                                else if ( ( topik == operator_com ) && !(check_E_before))
                                {
                                    check_E_before= true;
                                    stackPop(&zasobnik);
                                }
                                else
                                {
                                    printf("ERROR parameters in func \n");
                                    stackDelete_and_free(&zasobnik);
                                    printf("delete and free\n");
            //                        //print_stack_data(&zasobnik);
                                    return 0; // TODO break free stack
                                }
                                stackTop(&zasobnik,&topik);
                            }
                          //  printf("po cykle pocet param: %d\n", count_parameters);
          //                  //print_stack_data(&zasobnik);
                            if( topik == operator_pal)
                            {
                                stackPop(&zasobnik);
                                stackTop(&zasobnik,&topik);
                                if( topik == operator_fnc)
                                {
                                    stackPop(&zasobnik);

                                    if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik) )
                                        ;
                                    else
                                    {
                                        printf("error 123\n");// Todoooo error
                                        stackDelete_and_free(&zasobnik);
                                        printf("delete and free\n");
           //                             //print_stack_data(&zasobnik);
                                        return 0; // TODO break free stack
                                    }
                                }
                                else
                                {

                                    printf("error 123123\n");;//TODOooooooooooo error
                                    stackDelete_and_free(&zasobnik);
                                    printf("delete and free\n");
          //                          //print_stack_data(&zasobnik);
                                    return 0; // TODO break free stack
                                }
                            }
                            else
                            {
                                stackDelete_and_free(&zasobnik);
                                printf("delete and free lolik\n");
      //                          //print_stack_data(&zasobnik);
                                return 0; // TODO break free stack
                            }
                        }
                        else
                        {
                            stackDelete_and_free(&zasobnik);
                            printf("delete and free omgg\n");
     //                       //print_stack_data(&zasobnik);
                            return 0; // TODO break free stack
                        }
                            ;
                    }

              //todoooooooooooo
                }
                else if( topik == operator_NONTERM)
                {
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                    if( topik == operator_not)
                    {
                        printf("not\n");
                        stackPop(&zasobnik);
                        if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik) )
                            ;
                        else
                        {
                            printf("error 123\n");// Todoooo error
                            stackDelete_and_free(&zasobnik);
                            printf("delete and free\n");
        //                    //print_stack_data(&zasobnik);
                            return 0; // TODO break free stack
                        }
                        break;
                    }

                    switch(topik)
                    {
                      case operator_add:
                         // printf("add\n");
                          break;
                      case operator_sub:
                         // printf("sub\n");
                          break;
                      case operator_mul:
                         // printf("mul\n");
                          break;
                      case operator_div:
                         // printf("div\n");
                          break;
                      case operator_mod:
                         // printf("mod\n");
                          break;

                          // ++
                          // --

                      case operator_equ:
                         // printf("equ\n");
                          break;
                      case operator_neq:
                         // printf("neq\n");
                          break;
                      case operator_gre:
                       //   printf("gre\n");
                          break;
                      case operator_les:
                         // printf("les\n");
                          break;
                      case operator_goe:
                        //  printf("goe\n");
                          break;
                      case operator_loe:
                        //  printf("loe\n");
                          break;

                      case operator_or:
                        //  printf("or\n");
                          break;
                      case operator_and:
                        //  printf("and\n");
                          break;
                      case operator_dot:
                         // printf("dot\n");
                          break;
                      //case operator_not:
                      //    printf("not\n");
                      //    break;
                      default:
                          printf("ERROR default other symbol then +-*/ \n");
                          stackDelete_and_free(&zasobnik);
                          printf("delete and free\n");
      //                    //print_stack_data(&zasobnik);
                          return 0; // TODO break free stack
                          break;
                    }
                    stackPop(&zasobnik);
                    check_rule(&zasobnik,topik);
                }
                else
                {
                    stackDelete_and_free(&zasobnik);
                    printf("delete and free ...\n");
    //                //print_stack_data(&zasobnik);
                    return 0; // TODO break free stack
                }
                break;

            case symbol_fault:
                stackDelete_and_free(&zasobnik);
                printf("delete and free ==\n");
     //           //print_stack_data(&zasobnik);
                return 0; // TODO break free stack
        }
        stackTop(&zasobnik,&topik);
        //DBGG
       //print_stack_data(&zasobnik);
        //
    }while( My_operator != operator_dlr || !(topik == operator_NONTERM && (get_Top_operator_from_stack(&zasobnik) == operator_dlr) ) );
    stackDelete_and_free(&zasobnik);
    //print_stack_data(&zasobnik);
    get_back_token(token);
  return 1;
}


void check_rule(IntStack* stack,operators Rule_symbol)
{	
	
	Rule_symbol = Rule_symbol; // <= unused!!
	
    int topik;
    stackTop(stack,&topik);
    if(topik == operator_NONTERM)
    {
        stackPop(stack);
        if( is_TOP_symbol_less_POP_and_PUSH_Nonterm(stack) )
            ;
        else
            printf("ERROR 1- check rule\n");// TODO
    }
    else
    {
        printf("ERROR 2- check rule\n");// TODO
    }

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
    if ( pom == operator_NONTERM)
    {
        stackPop(stack);
        stackPush(stack,symbol_less);
        stackPush(stack,operator_NONTERM);
    }
    else
    {
        stackPush(stack,symbol_less);
    }

}

bool is_TOP_symbol_less_POP_and_PUSH_Nonterm(IntStack* stack)
{
    int topik;
    stackTop(stack,&topik);
    if( topik == symbol_less)
    {
        stackPop(stack);
        stackPush(stack,operator_NONTERM);
        return true;
    }
    else
        return false;

}
