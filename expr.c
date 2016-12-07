#include "expr.h"
#include "str.h"
#include "struct.h"
#include "lex.h"
#include "generator.h"
#include "instrlist.h"
#include "ial.h"

//TODO DBG DELETE
#include "print_stack.h"

//EXPr - konrola parametrov pri move

int expr_offset;
int count_of_const;
int count_ifjfn_parameters;
int token_vestaveneFN;

const char Precedence_table[P_table_size][P_table_size]={
//      "unary-"  "+"   "-"   "*"   "/"   "%"  "E++" "E--"  "=="  "!="  ">"   "<"   ">="  "<="  "||"  "&&"  "!"   "("   ")"   ","   "ID"  "Fn"  "$"   ++E  "--E"
/*unry-*/ { 'F' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , 'F' , '<' , '<' , '>' , '<' , '<' }, //unr-// UNARY
/*  +  */ { '<' , '>' , '>' , '<' , '<' , '<' , '>' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // +  //
/*  -  */ { '<' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // -  //
/*  *  */ { '<' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // *  //
/*  /  */ { '<' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // /  //
/*  %  */ { '<' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // %  // MODULO FOR FUTURE
/*E++  */ { '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '#' , '#' , '#' , '>' , '#' , '#' }, //E++ // UNARY
/*E--  */ { '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '#' , '#' , '#' , '>' , '#' , '#' }, //E-- // UNARY
/* ==  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // == //
/* !=  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // != //
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // >  //
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // <  //
/* >=  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // >= //
/* <=  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // <= //
/* ||  */ { '<' , '>' , '>' , '<' , '<' , '<' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // || //
/* &&  */ { '<' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '<' , '>' , '>' , '<' , '<' , '>' , '<' , '<' }, // && //
/*  !  */ { '<' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '<' , '>' , '>' , '<' , '<' , '>' , '#' , '#' }, // !  //
/*  (  */ { '<' , '<' , '<' , '<' , '<' , '<' , '#' , '#' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '=' , '<' , '<' , '>' , '<' , '<' }, // (  //
/*  )  */ { 'F' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , '>' , 'F' , 'F' , '>' , '#' , '#' }, // )  //
/*  ,  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '=' , '<' , '<' , 'F' , '<' , '<' }, // ,  //
/* ID  */ { 'F' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'F' , '>' , '>' , 'F' , 'F' , '>' , '#' , '#' }, // ID //
/* Fn  */ { 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , 'F' , '=' , 'F' , '>' , 'F' , 'F' , 'F' , '#' , '#' }, // Fn //
/*  $  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , 'F' , 'F' , '<' , '<' , 'F' , '<' , '<' }, // $  //
/*++E  */ { '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '<' , '<' , '>' , '#' , '#' }, // ++E // UNARY prefix
/*--E  */ { '#' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '#' , '>' , '>' , '<' , '<' , '>' , '#' , '#' }, // --E // UNARY prefix
};

operators fn_token_to_operators(expr_in help, int *count_paranthesis,String * key_identifier,IntStack *stack)
{
    int top_of_stack;
    switch(token->type)
    {
        case token_add:
            return operator_add;          // number 2 // "+" // Addition
        case token_sub:
            //OK-UNARY spraveme unarne minus
            stackTop(stack,&top_of_stack);
            if( top_of_stack == operator_NONTERM || top_of_stack == operator_fnc || top_of_stack == operator_ID || top_of_stack == operator_par )
                 return operator_sub;          // number 3 // "-" // Subtraction
            else
               return operator_una;          // number 0 // "-" // unary -
        case token_mul:
            return operator_mul;          // number 4 // "*" // Multiplication
        case token_div:
            return operator_div;          // number 5 // "/" // Division

////NO USE//////////////////////////////////////////////////////////////
        case token_mod:                   //token    number 6 // "%" // Modulo
            return operator_mod;          //operator number 5 // "%" // Modulo
////NO USE//////////////////////////////////////////////////////////////

        case token_inc:                           //token number 7 // "++" // Increment
            stackTop(stack,&top_of_stack);
            if( top_of_stack == operator_NONTERM || top_of_stack == operator_fnc || top_of_stack == operator_ID || top_of_stack == operator_par )
            {
                return operator_Einc;         //operator number 6 // "++" // Postfix Increment
            }
            else
                return operator_incE;         //operator number 23 // "++" // Prefix  Increment
        case token_dec:                       //token number 8 // "--" // Decrement
            stackTop(stack,&top_of_stack);
            if( top_of_stack == operator_NONTERM || top_of_stack == operator_fnc || top_of_stack == operator_ID || top_of_stack == operator_par )
            {
                return operator_Edec;      //operator number  // "--" // Postfix Decrement
            }
            else
                return operator_decE;      // number  // "--" // Decrement

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
            if( *count_paranthesis == 0 && (help != in_assign_first && help != in_assign_second) )
                return operator_dlr;          // number 19 // ")" // Parenthesis right
            else
                return operator_par;

        case token_com:
            return operator_com;          // number 20 // "," // Comma

        case token_identifier:

            strLoad(key_identifier,token->value);
            get_token(token,sourceFile);
            if( token->type == token_pal )
            {
                get_back_token(token);
                token_vestaveneFN=token_func;
                return operator_fnc;
            }
            else if(token_dot == token->type)
            {
                get_token(token,sourceFile);
                if( token->type != token_identifier )
                {
                    error_f(ERROR_SYNTAX); //TODO ERROR syntax ?
                }
                strCat(key_identifier,"_");
                strCat(key_identifier,token->value);

                get_token(token,sourceFile);
                if( token->type == token_pal )
                {
                    get_back_token(token);
                    token_vestaveneFN=token_func;
                    return operator_fnc;
                }
                else
                {
                    get_back_token(token);
                    return operator_ID;
                }
            }
            else
            {
                get_back_token(token);
                return operator_ID;
            }

        case token_iin    :           //0parametre //token number 61 // ifj16.readInt
        case token_din    :           //0parametre //token number 62 // ifj16.readDouble
        case token_sin    :           //0parametre //token number 63 // ifj16.readString

        case token_length :           //1parameter //token number 65 // ifj16.length
        case token_substr :           //3parametre //token number 66 // ifj16.substr
        case token_compare:           //2parametre //token number 67 // ifj16.compare
        case token_find   :           //2parametre //token number 68 // ifj16.find
        case token_sort   :           //1parameter //token number 70 // ifj16.sort
            token_vestaveneFN=token->type;
            return operator_fnc;      //operator number  // ID

        case token_out    :           //           //token number 64 // ifj16.print
            fprintf(stderr,"ERROR in Expr: funkcia ifj16.print in expr.\n");
            error_f(ERROR_SEM_OTHER);//TODO print funkcia vo vyraze aka chya ?

        case token_String:
        case token_true:
        case token_false:
        case token_number_int:
        case token_number_double:
            return operator_ID;            // number  // ID

 //TODO       case token_zlz: // {
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

int fn_expression(expr_in help,value_type General_result_type, ht_item_func_ptr fnce, T_instr_list  ** instrList)
{
    expr_offset=actFunctionOffset;

    String * key_ptr;
    key_ptr= (String *) memory_manager_malloc( sizeof(String) );
    strInit(key_ptr);

    String * buffer;
    buffer= (String *) memory_manager_malloc( sizeof(String) );

    int count_parameters=0;
    ExpStack * Help_ExpStack_ptr= memory_manager_malloc(sizeof(ExpStack));
    ExpStackInit(Help_ExpStack_ptr,STACK_SIZE);

    if( (firstScan && help == in_assign_first) || ( !firstScan && help != in_assign_first ) )
    {
        if(*instrList == NULL)
        {
            *instrList= memory_manager_malloc(sizeof(T_instr_list));
            listInit(*instrList);
        }
    }

    ht_table_item_ptr itemik_ptr=NULL;

    precedence_symbol symbol_from_table;
    operators My_operator;
    char c;
    IntStack zasobnik;
    count_of_const=0;
    int topik;

    int count_paranthesis;

    if( help == in_assign_first || help == in_assign_second )
    {
        count_paranthesis=0;
    }
    else
        count_paranthesis=1;

//TODO ALL fnce ->VOID
// RETURN ;
    get_token( token, sourceFile );
    if( help == in_return && token->type == token_sem )
    {
        get_back_token(token);
        return 1;
    }
    else

    //SPRACOVANIE PARAMETROV PRI VOLANI FNCE
    if( help == in_function && !firstScan)
    {
        if(fnce == NULL)//TODO ok? preistotu
            error_f(ERROR_SEM_UNDEFINED);//TODO check ERROR

        if(fnce->nParams > 1)
        {//napushujem parametre(na vrchol stacku naprazdno) ktore predpokladam ze najdem
            ht_params_ptr parametrik=fnce->param;
            for(int i=0; i < fnce->nParams; i++)
            {
                if(parametrik == NULL)
                    break;//error_f(ERROR_SEM_UNDEFINED)TODO !!!!  co ked parameter nebude a pocet bude vacsi ?

                T_address_code* code_f=NULL;
                code_f =  memory_manager_malloc(sizeof(T_address_code));
                T_variable *empty_var_f=NULL;
                empty_var_f = memory_manager_malloc(sizeof(T_variable));
                empty_var_f->type=parametrik->type; //TODO or OK ?
                empty_var_f->value.value_int=-1; //TODO param

                code_f->address1 = empty_var_f;
                T_address* adresa_f=NULL;
                adresa_f = memory_manager_malloc(sizeof(T_address));
                adresa_f->isGlobal=false; //s ktorym zasobnikom pracuje
                code_f->result = adresa_f;
                code_f->operation = T_PUSH;
                listInsert(*instrList, code_f);

                parametrik=parametrik->next;
                expr_offset++;
            }
        }
    }

    stackInit(&zasobnik,STACK_SIZE);
    stackPush(&zasobnik,operator_dlr);

    My_operator= fn_token_to_operators( help, &count_paranthesis, key_ptr ,&zasobnik);

    if ( (help == in_function) && (token->type == token_pal) )
    {
        stackPush(&zasobnik, symbol_less);
        stackPush(&zasobnik, operator_fnc);
    }

    do
    {

        if(My_operator == operator_NONTERM)
        {
            stackDelete_and_free(&zasobnik);
            fprintf(stderr,"delete and free 000\n");
            error_f(ERROR_SYNTAX);
        }
        c= Precedence_table[ get_Top_operator_from_stack( &zasobnik ) ][ My_operator ];
        symbol_from_table= fn_char_to_numsymbol(c);

        switch(symbol_from_table)
        {
            case symbol_equal:
                stackPush(&zasobnik,My_operator);
                get_token( token, sourceFile );
                My_operator= fn_token_to_operators( help , &count_paranthesis , key_ptr, &zasobnik);
                break;

            case symbol_less:
                add_less_operator_to_stack(&zasobnik);
                stackPush(&zasobnik,My_operator);

                if( (firstScan && help == in_assign_first) || ( !firstScan && help != in_assign_first ) )
                {

                    /*if(help == in_function)
                    {
                        ht_params_ptr help_param_ptr=fnce->param;
                        for(int i=0; i < count_parameters; i++)
                        {
                            help_param_ptr=help_param_ptr->next;
                        }
                        result_type=help_param_ptr->type;
                    }
                    else
                        result_type=General_result_type;
                    */


                    if(token->type == token_number_int || token->type == token_number_double || token->type == token_String )
                    {// CREATE CONSTANT and PUSH
                        T_variable * variablee_const=NULL;
                        variablee_const=memory_manager_malloc(sizeof(T_variable));

                        create_const(token,variablee_const);
/// //////////////////
/*
String *myString =NULL;
    switch(token->type)
    {
        case token_number_int:
            variablee_const->type= INT;
            variablee_const->value.value_int= atoi(token->value);
            break;
        case token_number_double:
            variablee_const->type = DOUBLE;
            variablee_const->value.value_double = atof(token->value);
            break;
        case token_String:
            myString = memory_manager_malloc(sizeof(String));
            strInit(myString);
            strLoad(myString, token->value);
            variablee_const->type = STRING;
            variablee_const->value.value_String = myString;
            break;
        default:
            error_f(ERROR_OTHER);
    }
*/
///  ////////////

                        //PUSH
                        T_address_code *code_c=NULL;
                        T_address * adresa_c=NULL;
                        code_c =  memory_manager_malloc(sizeof(T_address_code));
                        code_c->address1 = variablee_const;

                        adresa_c = memory_manager_malloc(sizeof(T_address));
                        adresa_c->isGlobal=false; //s ktorym zasobnikom pracuje
                        code_c->result = adresa_c;
                        code_c->operation = T_PUSH;
                        listInsert(*instrList, code_c);

                        //PUSH na Help_ExpStack
                        ExpStackPush(Help_ExpStack_ptr, variablee_const->type, expr_offset,CONSTANT);
                        expr_offset++;
                        count_of_const++;
// TODO //variablee_const=NULL;

                    }
                    else if(token->type == token_true || token->type == token_false)
                    {// CREATE CONSTANT and PUSH
                        T_variable * variablee_const=NULL;
                        variablee_const=memory_manager_malloc(sizeof(T_variable));
                        create_const(token,variablee_const);
/// /////////////
/*
case token_true:
            variablee_const->type = BOOLEAN;
            variablee_const->value.value_bool= 1;
            return;
        case token_false:
            var->type= BOOLEAN;
            var->value.value_bool= 0;
            return;
*/
/// /////////////
                        //PUSH
                        T_address_code *code_c=NULL;
                        T_address * adresa_c=NULL;
                        code_c =  memory_manager_malloc(sizeof(T_address_code));
                        code_c->address1 = variablee_const;

                        adresa_c = memory_manager_malloc(sizeof(T_address));
                        adresa_c->isGlobal=false; //s ktorym zasobnikom pracuje
                        code_c->result = adresa_c;
                        code_c->operation = T_PUSH;
                        listInsert(*instrList, code_c);

                        //PUSH na Help_ExpStack
                        ExpStackPush(Help_ExpStack_ptr, BOOLEAN, expr_offset, CONSTANT);
                        expr_offset++;
                        count_of_const++;
                    }
                    else if(My_operator == operator_ID)
                    {// FIND ID and (push on helpStack)
                        if(help == in_assign_first) //pri prvom prechode hladam len v lokalnej tabulke globalna neexistuje
                            itemik_ptr= NULL;
                        else
                            itemik_ptr= ht_search(key_ptr->str, false, local_table);
                        if(itemik_ptr == NULL)
                        {

                            itemik_ptr= ht_search(key_ptr->str, false, global_table);
                            if(itemik_ptr == NULL)
                            {
                                strInit(buffer);
                                strCopy(buffer,key_ptr);
                                strLoad(key_ptr,actTrieda->str);
                                strCat(key_ptr,"_");
                                strCat(key_ptr,buffer->str);
                                strFree(buffer);
                                itemik_ptr= ht_search(key_ptr->str, false, global_table);
                            }

                            if(itemik_ptr != NULL)
                            {
                                if(!itemik_ptr->var->inicialized)
                                    error_f(ERROR_UNINIT_VAR);
                                ///PUSH na ExpStack
                                ExpStackPush(Help_ExpStack_ptr, itemik_ptr->var->type, itemik_ptr->var->offset, ADRESS_G);
                            }
                            else
                            {
                                fprintf(stderr,"NO search variable\n");//TODO DELETE
                                error_f(ERROR_SEM_UNDEFINED);//TODO check ok ?
                            }
                        }
                        else
                        {
                            if(!itemik_ptr->var->inicialized)
                                error_f(ERROR_UNINIT_VAR);
                            ///PUSH na ExpStack
                            ExpStackPush(Help_ExpStack_ptr, itemik_ptr->var->type, itemik_ptr->var->offset, ADRESS_L);
                        }

                    }                                   //vestavene funkce sa nevyhladavaju
                                                        //token->type == token_pal - for all function
                    else if(My_operator == operator_fnc && token_vestaveneFN == token_func)
                    {//FIND FUNCTION
                        itemik_ptr= NULL;
                        if(help == in_assign_first)
                        {
                            error_f(ERROR_OTHER);
                            //TODO ERR CODE funkcia pri prvom prechode aka chyba???
                        }
                        itemik_ptr= ht_search(key_ptr->str, true, global_table);
                        if(itemik_ptr == NULL)
                        {
                                strInit(buffer);
                                strCopy(buffer,key_ptr);
                                strLoad(key_ptr,actTrieda->str);
                                strCat(key_ptr,"_");
                                strCat(key_ptr,buffer->str);
                                strFree(buffer);
                                itemik_ptr= ht_search(key_ptr->str, true, global_table);
                        }
                        if(itemik_ptr != NULL)
                        {
                            //start PUSH na prazdno +push na helpstack
                            //TODO VOID funkcia nepushujem naprazdno ???

                            T_address_code * code_e=NULL;
                            T_address *adresa_e=NULL;
                            T_variable *empty_var=NULL;

                            ///PUSH na prazdno - vysledok funkcie -> offset vysledku
                            code_e =  memory_manager_malloc(sizeof(T_address_code));
                            empty_var = memory_manager_malloc(sizeof(T_variable));

                            empty_var->type = itemik_ptr->func->type;
                            empty_var->value.value_int=-400; //TODO ZERO

                            code_e->address1 = empty_var;
                            adresa_e = memory_manager_malloc(sizeof(T_address));
                            adresa_e->isGlobal=false; //s ktorym zasobnikom pracuje
                            code_e->result = adresa_e;
                            code_e->operation = T_PUSH;
                            listInsert(*instrList, code_e);

                            //PUSH na ExpStack - vysledok funkcie -> offset vysledku
                            ExpStackPush(Help_ExpStack_ptr, itemik_ptr->func->type, expr_offset, ADRESS_L);
                            expr_offset++;
                            count_of_const++;
                            ///end PUSH na prazdno

                            int pom_offset=actFunctionOffset;
//TODO DELETE                  //int pom_expoffset=expr_offset;
                            actFunctionOffset=expr_offset;
                            ///PUSHOVANIE parametrov funkcie - volam sam seba ( ) -spracovanie parametrov
                            fn_expression(in_function, 0 ,itemik_ptr->func,instrList);
                            actFunctionOffset=pom_offset;
//TODO DELETE                  //expr_offset=pom_expoffset;

                            ///INST- FNCE -> generujem instrukciu call function
                            T_address * adresa2= memory_manager_malloc(sizeof(T_address) );
                            if(itemik_ptr->func->type == token_void ) //DOHODA ze o tom ci funkcia je void alebo nie rozhoduje adresa2
                                adresa2->isGlobal=true;
                            else
                                adresa2->isGlobal=false;
                            gen_code(T_FUNC,itemik_ptr->func->instructionList,adresa2,NULL, *instrList);

                            ///POP popujem parametre prec zo stacku
                            for(int i=0 ; i < itemik_ptr->func->nParams ; i++)
                            {
                                T_address * adresa = memory_manager_malloc(sizeof(T_address));
                                adresa->isGlobal = false;
                                gen_code(T_POP,NULL,NULL,adresa, *instrList);
                            }

                            /// Parametre su spracovane nacitane preskocim to-> zredukujem
                            //Rozdiel ako pri prvom prechode
                            // <  f
                            stackPop(&zasobnik);
                            is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                            // n

                        }
                        else
                        {
                            fprintf(stderr,"NO search fnce\n");
                            error_f(ERROR_SEM_UNDEFINED);//TODO check ok ?
                        }
                    }//END of FIND func/id
                }

                get_token( token, sourceFile );
                My_operator= fn_token_to_operators( help , &count_paranthesis , key_ptr, &zasobnik);
                break;

            case symbol_greater:
                stackTop(&zasobnik,&topik);
                //RULE 20 E-> i
                if(topik == operator_ID)
                {
                    stackPop(&zasobnik);
                    is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
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
                            is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
//TODO fnce bez parametrou
// move return;
                            if(!firstScan)
                            {
                                if(fnce == NULL)
                                {
                                    if( count_ifjfn_parameters != 0)
                                      error_f(ERROR_SEM_TYPE); //TODO 4 - sémantická chyba ... , príp. špatný pocet ci typ parametru u volání funkce.
                                }
                                else if( fnce->nParams != 0 )
                                    error_f(ERROR_SEM_TYPE); //TODO 4 - sémantická chyba ... , príp. špatný pocet ci typ parametru u volání funkce.
                            }
                        }
                        else
                        {

                            stackDelete_and_free(&zasobnik);
                            fprintf(stderr,"delete and free 002\n");
                            error_f(ERROR_SYNTAX);
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
                                stackPop(&zasobnik);
                                is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);

                                if( !firstScan && fnce->nParams != 1)
                                    error_f(ERROR_SEM_TYPE); //TODO 4 - sémantická chyba ... , príp. špatný pocet ci typ parametru u volání funkce.
                            }
                            // RULE E-> ( E )
                            else
                            {
                                is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                            }
                        }
                        // RULE E-> fn( E, E, ...)
                        else if( topik == operator_com)
                        {
                            //push koli zachovaniu poctu viacerych parametrov a spocitani ich
                            stackPush(&zasobnik,operator_NONTERM);
                            stackTop(&zasobnik,&topik);
                            count_parameters=0;
                            bool check_E_before= true;
                            while ( topik != operator_pal )
                            {
                                if ( ( topik == operator_NONTERM ) && check_E_before )
                                {
                                    check_E_before= false;
                                    count_parameters++;

                                    if(!firstScan)
                                    {//MOVE parameter na spravne miesto v poradi na stacku
                                        T_Exp_variable *Help_Exp_var_ptr= memory_manager_malloc( sizeof(T_Exp_variable) ); // mozem free potom
                                        ExpStackTop( Help_ExpStack_ptr, Help_Exp_var_ptr);

                                        T_address *adresa_1=memory_manager_malloc( sizeof(T_address) ); // odkial
                                        T_address *adresa_2=memory_manager_malloc( sizeof(T_address) ); // kam
                                        if( Help_Exp_var_ptr->GlobLokCons == ADRESS_G )
                                        {// GLOBAL_VAL;
                                            adresa_1->isGlobal= true;
                                        }
                                        else
                                        {// LOCAL_VAL alebo CONSTANT
                                            adresa_1->isGlobal=false;
                                        }
                                        adresa_1->offset=Help_Exp_var_ptr->offset;
                                        ExpStackPop(Help_ExpStack_ptr);
                                                          //expr_offset-fnce->nParams -(fnce->nParams- count_parameters)
                                                          //actFunctionOffset+ fnce->nParams - count_parameters
                                        adresa_2->offset= (actFunctionOffset - 1 + count_parameters);// od zaciatku "ramca" volania funkcie + pocet param
                                        adresa_2->isGlobal=false;
                                        gen_code(T_MOV,adresa_1,adresa_2,NULL,*instrList);

                                        if( Help_Exp_var_ptr->GlobLokCons == CONSTANT)
                                        {   // CONSTANT; - musim aj POP
                                            T_address *adrresult=memory_manager_malloc(sizeof(T_address)); // odkial
                                            adrresult->isGlobal=false;
                                            gen_code(T_POP,NULL,NULL,adrresult, *instrList);
                                            expr_offset--;
                                        }
                                        //TODO FREE
                                        //memory_manager_free_one(Help_Exp_var_ptr);
                                    }

                                    stackPop(&zasobnik);
                                }
                                else if( ( topik == operator_com ) && !(check_E_before) )
                                {
                                    check_E_before= true;
                                    stackPop(&zasobnik);
                                }
                                else
                                {
                                    fprintf(stderr,"ERROR parameters in func \n");
                                    stackDelete_and_free(&zasobnik);
                                    error_f(ERROR_SYNTAX); //TODO check OK ??
                                }
                                stackTop(&zasobnik,&topik);
                            }
                            if( !firstScan && fnce->nParams != count_parameters)
                                error_f(ERROR_SEM_TYPE); // • 4 - sémantická chyba ... , príp. špatný pocet ci typ parametru u volání funkce.

                            if( topik == operator_pal)
                            {
                                stackPop(&zasobnik);
                                stackTop(&zasobnik,&topik);
                                if( topik == operator_fnc)
                                {
                                    stackPop(&zasobnik);
                                    is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                                }
                                else
                                {
                                    fprintf(stderr,"error 123123\n");
                                    stackDelete_and_free(&zasobnik);
                                    error_f(ERROR_SYNTAX);
                                }
                            }
                            else
                            {
                                stackDelete_and_free(&zasobnik);
                                fprintf(stderr,"delete and free lolik\n");
                                error_f(ERROR_SYNTAX);
                            }
                        }
                        else
                        {
                            stackDelete_and_free(&zasobnik);
                            fprintf(stderr,"delete and free omgg\n");
                            error_f(ERROR_SYNTAX);
                        }
                    }
                    else
                    {//TODO
                        fprintf(stderr,"Erroriik\n");
                        error_f(ERROR_SYNTAX);
                    }//TODO

                }//UNARY postfix E++ E--
                else if(topik == operator_Edec || topik == operator_Einc)
                {//TODO make pushvariable stack and push to this stack offset of local or global variable constat throw uot
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                    if(topik == operator_NONTERM )
                    {
//POSTFIX TODOO
                        stackPop(&zasobnik);
                        is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                        break;
                    }
                    else
                        error_f(ERROR_SYNTAX);

                }
                // UNARY: E-> ++E , E-> --E
                // E-> !E  E-> -E unarne minus
                // E-> E +-*... E
                else if( topik == operator_NONTERM)
                {
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);

                    // UNARY: ++E, --E
                    if( topik == operator_decE || topik == operator_incE )
                    {

                        if( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) )
                            UNARY_gen_prefix(Help_ExpStack_ptr, *instrList, topik);

                        stackPop(&zasobnik);
                        is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                        break;
                    }
                    // E-> -E UNARY unarne minus
                    // E-> !E
                    else if( topik == operator_una || topik == operator_not)
                    {
                        if( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) )
                            gen_NOT_UNM( Help_ExpStack_ptr, *instrList,topik );

                        stackPop(&zasobnik);
                        is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                        break;
                    }

                    switch(topik)
                    {
                      // E-> E+E
                      case operator_add:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_add,Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E-E
                      case operator_sub:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_sub,Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E*E
                      case operator_mul:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_mul, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E/E
                      case operator_div:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_div,Help_ExpStack_ptr, *instrList);
                          break;

///////////NO USE /////////////////////////////////////////////
                      // E-> E%E
                      case operator_mod:
                          // fprintf(stderr,"mod\n");
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_mod, Help_ExpStack_ptr, *instrList);
                          break;
///////////NO USE /////////////////////////////////////////////

                      // E-> E==E
                      case operator_equ:
                          if( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) )
                              set_adress_and_gen_code(token_equ, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E!=E
                      case operator_neq:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_neq, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E>E
                      case operator_gre:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_gre, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E<E
                      case operator_les:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_les, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E>=E
                      case operator_goe:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_goe, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E<=E
                      case operator_loe:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_loe, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E||E
                      case operator_or:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_or, Help_ExpStack_ptr, *instrList);
                          break;
                      // E-> E&&E
                      case operator_and:
                          if(( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) ))
                              set_adress_and_gen_code(token_and, Help_ExpStack_ptr, *instrList);
                          break;
                      default:
                          fprintf(stderr,"ERROR default other symbol then +-*/ \n");
                          stackDelete_and_free(&zasobnik);
                          error_f(ERROR_SYNTAX);
                          break;
                    }
                    stackPop(&zasobnik);
                    stackTop(&zasobnik,&topik);
                    if(topik == operator_NONTERM)
                    {
                        stackPop(&zasobnik);
                        is_TOP_symbol_less_POP_and_PUSH_Nonterm(&zasobnik);
                    }
                    else
                    {
                        fprintf(stderr,"ERROR 2- check rule\n");
                        error_f(ERROR_SYNTAX);
                    }
                }
                else
                {
                    stackDelete_and_free(&zasobnik);
                    fprintf(stderr,"delete and free ...\n");
                    error_f(ERROR_SYNTAX);
                }
                break;

            case symbol_fault:
                stackDelete_and_free(&zasobnik);
                fprintf(stderr,"delete and ==\n");
                error_f(ERROR_SYNTAX);
        }
        stackTop(&zasobnik,&topik);
        // TODO delete DBGG
        FILE *milosko=fopen("milosko.txt","a");
        print_stack_data(&zasobnik, milosko);
        fclose(milosko);
        //
    }while( My_operator != operator_dlr || !(topik == operator_NONTERM && (get_Top_operator_from_stack(&zasobnik) == operator_dlr) ) );

    if( (firstScan && help == in_assign_first) || ( !firstScan && help !=in_assign_first ) )
    {
        if( !ExpStackEmpty(Help_ExpStack_ptr) && count_of_const == 0 )
        {//PRIRADENIE JEDNEJ PREMENNEJ
            T_Exp_variable *variablee_top_ptr=memory_manager_malloc(sizeof(T_Exp_variable)); // mozem free potom
            ExpStackTop(Help_ExpStack_ptr, variablee_top_ptr);

            T_address *adresa_1=memory_manager_malloc(sizeof(T_address)); // odkial
            T_address *adresa_2=memory_manager_malloc(sizeof(T_address) ); // kam

            if( variablee_top_ptr->GlobLokCons == ADRESS_G )
                // GLOBAL_VAL;
                adresa_1->isGlobal=true;
            else if( variablee_top_ptr->GlobLokCons == ADRESS_L )
                // LOCAL_VAL;
                adresa_1->isGlobal=false;
            //else
            //TODO I DONT KNOW toto nemoze nastat

            adresa_1->offset=variablee_top_ptr->offset;
            value_type sem_asg_mov=variablee_top_ptr->type;
//SEM OK to PUSH
            ExpStackPop(Help_ExpStack_ptr);

//TODO !!!
        //INS prazdny PUSH
        adresa_2->isGlobal=false;
        T_variable *empty_var= memory_manager_malloc(sizeof(T_variable) );
        empty_var->type=sem_asg_mov;
        empty_var->value.value_int=0;
        gen_code(token_push,empty_var,NULL,adresa_2, *instrList);

            adresa_2->offset=expr_offset;

//TODO SEM RULE pri 1 PREMENNEJ
            gen_code(T_MOV,adresa_1,adresa_2,NULL,*instrList);
            expr_offset++;
        }
    }

    if( ( !firstScan && help == in_if ) || ( !firstScan && help == in_while ) )
    {
        T_address * adrresult= memory_manager_malloc(sizeof(T_address));
        adrresult->isGlobal=false;
        gen_code(T_POP,NULL,NULL,adrresult, *instrList);
        expr_offset--;
    }

    //TODO
    //stackDelete_and_free(&zasobnik);
    //ExpStackDelete_and_free(Help_ExpStack_ptr);
    //strFree(key_ptr);
    //memory_manager_free_one(buffer); // strFree buffer uvolnujem hned po pouziti

    get_back_token(token);
    return 1;
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

void is_TOP_symbol_less_POP_and_PUSH_Nonterm(IntStack* stack)
{
    int topik;
    stackTop(stack,&topik);
    if( topik == symbol_less)
    {
        stackPop(stack);
        stackPush(stack,operator_NONTERM);
    }
    else
    {
        fprintf(stderr,"ERROR is_TOP_symbol_less_POP_and_PUSH_Nonterm\n");
        error_f(ERROR_SYNTAX);
    }

}

void gen_code(int operation, void *address1, void *address2, void *result, T_instr_list *L_ins)
{
    T_address_code *ins = memory_manager_malloc(sizeof(T_address_code));
    ins->operation = operation;
    ins->address1 = address1;
    ins->address2 = address2;
    ins->result = result;

    listInsert(L_ins, ins);
}

value_type SEM_check_operation_return_Result_Type(value_type sem1,value_type sem2, token_type Operations)
{

        if( (sem1 == STRING || sem2 == STRING) && Operations == token_add)
        {
            return STRING; // KONKATENACE
        }
        else if(sem1 == STRING )
        {
            if(Operations != token_add)
            {
                fprintf(stderr,"ERROR in EXR operacie so STRING ina ako + \n");
                error_f(ERROR_SEM_TYPE);
            }
        }
        else if(sem1 == BOOLEAN)
        {
            //TODO check operations with BOOLEAN
            if( sem2 == BOOLEAN )
            {                                                           //not sa vyhodnocuje inde
                if( Operations == token_equ || Operations == token_neq || Operations == token_and ||  Operations == token_or)
                    return BOOLEAN;
                else
                {
                    fprintf(stderr,"ERROR in EXR operacia s BOOLEAN ine ako E==E, E!=E, E&&E, E||E, !E.\n");
                    error_f(ERROR_SEM_TYPE);
                }
            }
            else//OPERACIE S INT ??
            {
                fprintf(stderr,"ERROR in EXR operacia s BOOLEAN /*-+.. INT/DOUBLE/STRING\n");
                error_f(ERROR_SEM_TYPE);
            }
        }
        else if(sem1 == INT )
        {
            if( sem2 == INT)
            {
                return INT;
            }
            else if( sem2 == DOUBLE)
            {
                return DOUBLE;
            }
            else
            {
                fprintf(stderr,"ERROR in EXR operacia s INT /*-+.. STRING/BOOLEAN\n");
                error_f(ERROR_SEM_TYPE);
            }
        }
        else if(sem1 == DOUBLE )
        {
            if( sem2 == INT || sem2 == DOUBLE)
            {
                return DOUBLE;
            }
            else
            {
                fprintf(stderr,"ERROR in EXR operacia s DOUBLE /*-+.. STRING/BOOLEAN\n");
                error_f(ERROR_SEM_TYPE);
            }
        }
        else
        {///NEMOZE NASTAT !!!
            fprintf(stderr,"ERROR in EXR : fn-> SEM_check_operation() \n");
            error_f(ERROR_SEM_OTHER);
        }

        return INT;///NEMOZE NASTAT !!!-> warning
}

/**
* SEMATIKA
**/
void set_adress_and_gen_code(token_type operation,ExpStack *Help_ExpStack_ptr, T_instr_list  * MyinstrList)
{
// result type  call before gen operation SEM_check_operation(value_type sem1,value_type sem2, value_type result_type)
    T_address_code *code= NULL;
    T_address *adresa;
    T_address *adresa1= memory_manager_malloc(sizeof(T_address));
    T_address *adresa2= memory_manager_malloc(sizeof(T_address));
    T_address *result= memory_manager_malloc(sizeof(T_address));
    result->isGlobal=true; // pomocny priznak ze prve ID nieje konstanta

    T_Exp_variable *topiiiiik;
    topiiiiik=memory_manager_malloc(sizeof(T_Exp_variable));

    ExpStackTop(Help_ExpStack_ptr, topiiiiik);
    value_type sem1=topiiiiik->type;
    if( topiiiiik->GlobLokCons == ADRESS_G )
    //GLOBAL_VAL +*/- ...
        adresa1->isGlobal=true;
    else if( topiiiiik->GlobLokCons == ADRESS_L )
    //LOCAL_VAL +*/- ...
        adresa1->isGlobal=false;
    else
    {//KONSTANTA +*/- ...
        adresa1->isGlobal=false;
        result->isGlobal=false;
        result->offset=topiiiiik->offset;
    }
    adresa1->offset=topiiiiik->offset;
    ExpStackPop(Help_ExpStack_ptr);

    ExpStackTop(Help_ExpStack_ptr, topiiiiik);
    value_type sem2= topiiiiik->type;

    value_type semPART;
    semPART = SEM_check_operation_return_Result_Type(sem2, sem1, operation);

    if( topiiiiik->GlobLokCons == ADRESS_G )
    {//... -*/+ GLOBAL_VAL
        adresa2->isGlobal=true;
        adresa2->offset=topiiiiik->offset;
        ExpStackPop(Help_ExpStack_ptr);
        if(result->isGlobal)
        {
            result->isGlobal=false;

            result->offset=expr_offset;
            T_variable *empty_var;

            code =  memory_manager_malloc(sizeof(T_address_code));
            empty_var = memory_manager_malloc(sizeof(T_variable));
            empty_var->type = semPART;
            empty_var->value.value_int=0;

            code->address1 = empty_var;
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false; //s ktorym zasobnikom pracuje
            code->result = adresa;
            code->operation = T_PUSH;
            listInsert(MyinstrList, code);

//TODO SEM
            ExpStackPush(Help_ExpStack_ptr, semPART ,expr_offset, CONSTANT);
            expr_offset++;
            count_of_const++;
        }
        else
        {
            ExpStackPush(Help_ExpStack_ptr, semPART, result->offset-1, CONSTANT);
        }
        gen_code(operation,adresa2,adresa1,result,MyinstrList);
    }
    else if( topiiiiik->GlobLokCons == ADRESS_L )
    {// ... -*/+ LOCAL_VAL
        adresa2->isGlobal=false;
        adresa2->offset=topiiiiik->offset;
        ExpStackPop(Help_ExpStack_ptr);
        if(result->isGlobal)
        {
            result->isGlobal=false;

            result->offset=expr_offset;
            T_variable *empty_var;

            code =  memory_manager_malloc(sizeof(T_address_code));
            empty_var = memory_manager_malloc(sizeof(T_variable));
            empty_var->type = semPART;
            empty_var->value.value_int=0;

            code->address1 = empty_var;
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false; //s ktorym zasobnikom pracuje
            code->result = adresa;
            code->operation = T_PUSH;
            listInsert(MyinstrList, code);

            ExpStackPush(Help_ExpStack_ptr, semPART, expr_offset, CONSTANT);
            expr_offset++;
            count_of_const++;
        }
        else
        {
            ExpStackPush(Help_ExpStack_ptr, semPART, expr_offset-1, CONSTANT);
        }
        gen_code(operation,adresa2,adresa1,result,MyinstrList);
    }
    else
    {//... -*/+ KONSTANTA
      //TODO DOUBLE + int -> pop push nanovo ?
        gen_code(operation,adresa2,adresa1,adresa2,MyinstrList);
        if(!result->isGlobal)
        {
            result->isGlobal=false;
            gen_code(T_POP,NULL,NULL,result, MyinstrList);
            expr_offset--;
        }
        adresa2->isGlobal=false;
        adresa2->offset=topiiiiik->offset;
    }

}

void create_const(T_token* tokenik, T_variable* var )
{
    String *myString =NULL;
    switch(tokenik->type)
    {
        case token_number_int:
            var->type= INT;
            var->value.value_int= atoi(tokenik->value); //TODO call lukas function
            return;
        case token_number_double:
            var->type = DOUBLE;
            var->value.value_double = atof(tokenik->value); //TODO call lukas function
            return;
        case token_String:
            myString = memory_manager_malloc(sizeof(String));
            strInit(myString);
            strLoad(myString, tokenik->value);
            var->type = STRING;
            var->value.value_String = myString;
            return;
        case token_true:
            var->type = BOOLEAN;
            var->value.value_bool= 1;
            return;
        case token_false:
            var->type= BOOLEAN;
            var->value.value_bool= 0;
            return;
        default:
            return;
          //TODO some error ?
    }
}

/**
* OK
**/
void UNARY_gen_prefix(ExpStack* Help_ExpStack_ptr, T_instr_list *instrList_ptr, int topik)
{
      if( topik == operator_decE)
          topik=T_DEC;
      else if( topik == operator_incE)
          topik=T_INC;

        T_Exp_variable *variablee_top=memory_manager_malloc(sizeof(T_Exp_variable)); // mozem free potom
        T_address *adresa=memory_manager_malloc(sizeof(T_address));

        ExpStackTop(Help_ExpStack_ptr, variablee_top);
        // ++/-- GLOBAL_VAL
        if( variablee_top->GlobLokCons == ADRESS_G )
            adresa->isGlobal=true;
        // ++/-- LOCAL_VAL alebo ++/-- KONSTANTA
        else
            adresa->isGlobal=false;

        if(variablee_top->type != INT )
        {
            fprintf(stderr,"EXP prefixova operacia nieje nad INT hodnotou\n");
            error_f(ERROR_SEM_TYPE);
        }

        adresa->offset=variablee_top->offset;
        gen_code(topik,adresa,NULL,NULL, instrList_ptr);
        //memory_manager_free_one(variablee_top);//TODO DELETE ?
}
/**
* OK
**/
void gen_NOT_UNM( ExpStack* Help_ExpStack_ptr, T_instr_list *instrList_ptr, int topik )
{
    if(topik == operator_una)
        topik=token_unm;
    else if(topik == operator_not)
        topik=token_not;

    T_Exp_variable *variablee_top=memory_manager_malloc(sizeof(T_Exp_variable)); // mozem free potom
    ExpStackTop(Help_ExpStack_ptr, variablee_top);

    value_type sem1=variablee_top->type;

    if(topik == token_not && sem1 != BOOLEAN)
    {
        fprintf(stderr,"ERROR NEGACIA nad hodnotou ktora nieje typu BOOLEAN\n");
        error_f(ERROR_SEM_TYPE);
    }
    if(topik == token_unm && ( sem1 != INT || sem1 != DOUBLE) )
    {
        fprintf(stderr,"ERROR UNARY  unarne minus nad hodnotou ktora nieje typu INT alebo DOUBLE\n");
        error_f(ERROR_SEM_TYPE);
    }

    T_address *adresa=memory_manager_malloc(sizeof(T_address));
    T_address *adresa_result=memory_manager_malloc(sizeof(T_address) );

    if( variablee_top->GlobLokCons == ADRESS_G )
    {// ! - GLOBAL_VAL
        adresa->isGlobal=true;
        adresa->offset=variablee_top->offset;
        ExpStackPop(Help_ExpStack_ptr);

        adresa_result->isGlobal=false;
        T_variable *empty_var=memory_manager_malloc(sizeof(T_variable) );
        empty_var->type=sem1;
        empty_var->value.value_int=0;
        gen_code(token_push,empty_var,NULL,adresa_result, instrList_ptr);
//TODO PUSH - insert to instr list

        adresa_result->offset=expr_offset;
        ExpStackPush(Help_ExpStack_ptr, sem1, expr_offset, CONSTANT);

        gen_code(topik,adresa,NULL,adresa_result,instrList_ptr);
        expr_offset++;
        count_of_const++;
    }
    else if( variablee_top->GlobLokCons == ADRESS_L )
    {// ! - LOCAL_VAL

        adresa->isGlobal=false;
        adresa->offset=variablee_top->offset;
        ExpStackPop(Help_ExpStack_ptr);

        adresa_result->isGlobal=false;
        T_variable *empty_var=memory_manager_malloc(sizeof(T_variable) );
        empty_var->type=sem1;
        empty_var->value.value_int=0;
        gen_code(token_push,empty_var,NULL,adresa_result, instrList_ptr);

        adresa_result->offset=expr_offset;
        ExpStackPush(Help_ExpStack_ptr, sem1, expr_offset, CONSTANT);

        gen_code(topik,adresa,NULL,adresa_result, instrList_ptr);
        expr_offset++;
        count_of_const++;
    }
    else
    {// ! - KONSTANTA
        adresa->isGlobal=false;
        adresa->offset=variablee_top->offset;
        gen_code(topik,adresa,NULL,adresa,instrList_ptr);
    }
    //memory_manager_free_one(variablee_top); //TODO ??
}




void vestavene_fnce(token_type token_vestaveneFN )
{
//    token_vestaveneFN=token_vestaveneFN; // delete  warnning
    //pushnut navratovu hondotu
    //TODO co ak je to funkcia bez navratovej hodnoty ?
    //napushovat parametre
    // call fn_expression() ?? moze tam byt vyraz alebo napevno pracovat s parametrami
    //call fnce
    //pop parametre

    /*
        case token_iin    :           //0parametre //token number 61 // ifj16.readInt         //TODO moze byt vo vyraze ??
        case token_din    :           //0parametre //token number 62 // ifj16.readDouble
        case token_sin    :           //0parametre //token number 63 // ifj16.readString
        case token_out    :           //           //token number 64 // ifj16.print           //TODO moze byt vo vyraze ??
        case token_length :           //1parameter //token number 65 // ifj16.length
        case token_substr :           //3parametre //token number 66 // ifj16.substr
        case token_compare:           //2parametre //token number 67 // ifj16.compare
        case token_find   :           //2parametre //token number 68 // ifj16.find
        case token_sort   :           //1parameter //token number 70 // ifj16.sort


    T_address_code * code;
    T_address * adresa;
    T_variable * premenna;
    String *myString;

    if(token->type == token_iin)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            return false;
        get_token(token, sourceFile);
        if(token->type != token_par)
            return false;
        get_token(token, sourceFile);
        if(token->type != token_sem)
            return false;

        if(!firstScan)
        {
            code =  memory_manager_malloc(sizeof(T_address_code));
            code->operation = T_IIN;
            listInsert(instructionList, code);

            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false;
            code->result = adresa;
            code->operation = T_POP;
            listInsert(instructionList, code);
        }
        return true;
    }
    else if(token->type == token_din)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            return false;
        get_token(token, sourceFile);
        if(token->type != token_par)
            return false;
        get_token(token, sourceFile);
        if(token->type != token_sem)
            return false;

        if(!firstScan)
        {
            code =  memory_manager_malloc(sizeof(T_address_code));
            code->operation = T_DIN;
            listInsert(instructionList, code);

            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false;
            code->result = adresa;
            code->operation = T_POP;
            listInsert(instructionList, code);
        }
        return true;
    }
    else if(token->type == token_sin)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            return false;
        get_token(token, sourceFile);
        if(token->type != token_par)
            return false;
        get_token(token, sourceFile);
        if(token->type != token_sem)
            return false;

        if(!firstScan)
        {
            code =  memory_manager_malloc(sizeof(T_address_code));
            code->operation = T_SIN;
            listInsert(instructionList, code);

            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false;
            code->result = adresa;
            code->operation = T_POP;
            listInsert(instructionList, code);
        }
        return true;
    }
    else if(token->type == token_out)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            return false;

        functPrintParams(instructionList);

        get_token(token, sourceFile);
        if(token->type != token_par)
            return false;
        get_token(token, sourceFile);
         if(token->type != token_sem)
            return false;

        return true;
    }
    else if(token->type == token_length)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            return 0;
        get_token(token, sourceFile);
         if(token->type != token_String) //treba tu rozlisovat typy, treba rozlisit ci tam nieje remenna
            return false;

        if(!firstScan)
        {
            code =  memory_manager_malloc(sizeof(T_address_code));
            premenna = memory_manager_malloc(sizeof(T_variable));
            myString = memory_manager_malloc(sizeof(String));
            strInit(myString);
            strLoad(myString, token->value);

            premenna->type = STRING;
            premenna->value.value_String = myString;

            code->address1 = premenna;
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false; //s ktorym zasobnikom pracuje
            code->result = adresa;
            code->operation = T_PUSH;
            listInsert(instructionList, code);

            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false; //s ktorym zasobnikom pracuje
            adresa->offset = actFunctionOffset;
            code->address1 = adresa;
            code->operation = T_LENGTH;
            listInsert(instructionList, code);

            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false;
            code->result = adresa;
            code->operation = T_POP;
            listInsert(instructionList, code);
        }

        get_token(token, sourceFile);
        if(token->type != token_par)
            return false;
        get_token(token, sourceFile);
         if(token->type != token_sem)
            return false;

        return true;
    }
    else if(token->type == token_substr)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            return false;

  //       if(fn_expression(in_substr_fun,result_type,NULL, &instructionList)  != 1)
   //             break;

        return true;
    }
    else if(token->type == token_compare)
    {
        return true;
    }
    else if(token->type == token_find)
    {
        return true;
    }
    else if(token->type == token_sort)
    {
        return true;
    }
    return false;
}
*/

}
