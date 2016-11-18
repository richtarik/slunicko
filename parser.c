#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

#include "parser.h"
#include "lex.h"
#include "expr.h"
#include "ial.h"

hash_table_ptr global_table;
hash_table_ptr local_table;
String * actTrieda; //v akej triede som aktualne

int globalVarsOffset;
int actFunctionOffset;

int main()
{

    global_table = ht_init(100); //////ak to tam spadne, neuvolnim svoju pamet
    local_table = ht_init(100);

    globalVarsOffset=0;
    actFunctionOffset=0;

    actTrieda = memory_manager_malloc(sizeof(String));
    strInit(actTrieda);

    if(global_table == NULL || local_table == NULL)
    {
        return -1;
        //printf(stderr, "Malo pameti");
    }
    
    token = memory_manager_malloc(sizeof(T_token));
    if(token==NULL)
        return(-1); //nedostatok pamete
        
   sourceFile = fopen("vstup.txt","r");

   if(sourceFile == NULL)
   {
        //printf(stderr,"Cant open file");
        return -1;
   }

   Init_token(token);

   if(nontermClass(sourceFile) == 1)
   {
        printf("EVERYTHONG OK \n");
        //print_table(global_table);
   }
   else
   {
		error_f(ERROR_SYNTAX);
        //printf("SYNTAX_ERROR");
   }

    free_token(token);

    ht_free(global_table);
    ht_free(local_table);
    fclose(sourceFile);
    memory_manager_free_all();
}
int nontermClass()
{
    int result = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_class)
        {
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                break;

            strLoad(actTrieda,token->value);
            get_token(token, sourceFile);
            if(token->type != token_zlz)
                break;
            if(nontermVnutroTriedy() != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_zrz)
                break;
              if(nontermClass() != 1)
                break;
            if(token->type == token_EOF)
                result = 1;
        }
        else if(token->type == token_EOF)
            result = 1;
    }while(0);

    return result;

}

int nontermVnutroTriedy()
{
    int result = -1;
    String *key;

    key = (String*) memory_manager_malloc(sizeof(String));
    strInit(key);

    do
    {
       get_token(token, sourceFile);
       if(token->type == token_zrz) //je tam koniec bloku
       {
            result = 1;
            get_back_token(token);
            break;
       }
       if(token->type != token_static) //povinny static
            break;

       get_token(token, sourceFile);
       if(token->type == token_void) //osetrenie samostatnej vetvy ak je funkcia void
       {
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                break;
            strLoad(key, token->value);
            get_token(token, sourceFile);
            if(token->type == token_pal) //funkcia
            {

                if(nontermFunctionParams(key, token_void) != 1)
                    break;
                //get_token(&token, sourceFile);
                // if(token->type != token_par) // zatvorku skontrolujem vo functionParams
                //     break;
                get_token(token, sourceFile);
                if(token->type != token_zlz)
                    break;
                if(nontermVnutroFunkcie() != 1)
                    break;
                get_token(token, sourceFile);
                if(token->type != token_zrz)
                    break;
                if(nontermVnutroTriedy() != 1)
                    break;

                result = 1;
                break;
            }
            else
                break;

       }
       if(token->type != token_double && token->type != token_int && token->type != token_String_type)
            break;

        token_type actType;
        actType = token->type;

        get_token(token, sourceFile);

        if(token->type != token_identifier)
            break;
        ///
            strLoad(key, actTrieda->str);
            strCat(key, "_");
            strCat(key, token->value);
        ///pripravenie identifikatora

        if(nontermTriednyClen(key, actType) != 1)
            break;
        else
            result = 1;
    }while(0);

    return result;
}
int nontermTriednyClen(String *key,  token_type actType)
{
    int result = -1;
    bool premennaDefinition=false;
    bool premennaInit=false;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_pal) //funkcia
        {
            if(nontermFunctionParams(key, actType) != 1)
                break;
            //get_token(&token, sourceFile);
            // if(token->type != token_par) // zatvorku skontrolujem vo functionParams
            //     break;
            get_token(token, sourceFile);
            if(token->type != token_zlz)
                break;
            if(nontermVnutroFunkcie() != 1)
                break;
             get_token(token, sourceFile);
             if(token->type != token_zrz)
                break;
        }
        else if(token->type == token_asg) //premenna priradenie
        {
            if(fn_expression(sourceFile, in_assign) != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;



           premennaDefinition = true;
           premennaInit = true;
        }
        else if(token->type == token_sem)
        {
            premennaDefinition = true;
        }
        else
            break;

        ///zapis premennej do tabulky
        if(premennaDefinition)
        {
            ht_item_var_ptr item = ht_create_item_var(actType,globalVarsOffset,premennaInit);
            globalVarsOffset++;
            if(ht_insert_item_var(item, key->str, global_table) == false)
            {
                //printf(stderr,"Redeklaracia globalnej premennej");
                result = -1;
            }
        }
        ///zapis premennej do tabulky

        if(nontermVnutroTriedy() != 1)
                break;
        result = 1;

    }while(0);

    return result;
}
int nontermFunctionParams(String *key,  token_type actType)
{
    int result = -1;
    T_item_list *listItemPtr;
    ht_params_ptr ht_param = NULL;
    ht_params_ptr ht_next = NULL;
    ht_params_ptr ht_firstParam = NULL;
    token_type param_type;

    int paramsOffset = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_par)
        {
            result = 1;
            break;
        }

        //prvy parameter
        if(token->type != token_double && token->type != token_int && token->type != token_String_type)
            break;

        param_type = token->type;

        get_token(token, sourceFile);
        if(token->type != token_identifier)
            break;

        //ulozenie prveho parametra
        ht_param = (ht_params_ptr) memory_manager_malloc(sizeof(struct ht_params));
        ht_param->name = (char*) memory_manager_malloc(sizeof(char)*(token->valActsize));
        //strcpy(ht_param->name,token->value);

        ht_param->offset = paramsOffset;
        paramsOffset--;
        ht_param->type = param_type;
        ht_param->next=NULL;

        ht_firstParam = ht_param;
        //ulozenie prveho  parametra


        //dalsie parametre
        do
        {
            get_token(token, sourceFile);
            if(token->type == token_par)
            {
                result = 1;
                break;
            }
            if(token->type != token_com)
                break;
            get_token(token, sourceFile);
            if(token->type != token_double && token->type != token_int && token->type != token_String_type)
                break;
            param_type = token->type;
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                break;

            //ulozenie dalsieho parametra
            ht_next = (ht_params_ptr) memory_manager_malloc(sizeof(struct ht_params));
            ht_next->name = (char*) memory_manager_malloc(sizeof(char)*(token->valActsize));
            //strcpy(ht_next->name,token->value);

            ht_next->offset = actFunctionOffset;
            actFunctionOffset++;
            ht_next->type = param_type;
            ht_next->next=NULL;

            ht_param->next = ht_next;
            ht_param = ht_next;
            //ulozenie prveho  parametra

        }while(1);

    }while(0);

        String * functionName;
        functionName = (String*) memory_manager_malloc(sizeof(String));
        strInit(functionName);
        strLoad(functionName, actTrieda->str);
        strCat(functionName,"_");
        strCat(functionName,key->str);

		// nebylo inicializováno!!!
		listItemPtr = NULL;

      ///zapis do tabulky funkcie
        ht_item_func_ptr item = ht_create_item_func(actType,listItemPtr,ht_firstParam);
        if(ht_insert_item_func(item, functionName->str, global_table) == false)
        {
            //printf(stderr,"Redeklaracia globalnej funkcie");
            result = -1;
        }
        ///
    return result;
}
int nontermVnutroFunkcie()
{
    int result = -1;

    get_token(token, sourceFile);

    do
    {

        if(token->type == token_zrz)
        {
            result = 1;
            get_back_token(token);
            break;
        }
        else
            get_back_token(token);
        if(nontermStatement() != 1)
            break;
        if(nontermVnutroFunkcie() != 1)
            break;
        result = 1;
    }while(0);

    return result;
}
int nontermStatement()
{
    int result = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_if)
        {
            get_token(token, sourceFile);
            if(token->type != token_pal)
                break;
            if(fn_expression(sourceFile, in_if) != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_par)
                break;
            if(nontermStatementBody() != 1)
                break;
            if(nontermElse() != 1)
                break;
            result = 1;
        }
        else if(token->type == token_while)
        {
            get_token(token, sourceFile);
            if(token->type != token_pal)
                break;
            if(fn_expression(sourceFile, in_while) != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_par)
                break;
            if(nontermStatementBody() != 1)
                break;
            result = 1;
        }
        else if(token->type == token_identifier)
        {
            get_token(token, sourceFile);
            if(token->type == token_dot) //rozsireny vyraz
            {
                get_token(token, sourceFile);
                if(token->type != token_identifier)
                    break;
            }
            else
                get_back_token(token);
            if(nontermRozlisenieIdentifikatora() != 1)
                break;
            result = 1;
        }
        else if(token->type == token_double || token->type == token_int || token->type == token_String_type)
        {
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                break;
            get_token(token, sourceFile);
            if(token->type == token_asg)
            {
                if(fn_expression(sourceFile, in_assign) != 1)
                    break;
            }
            else
                get_back_token(token);

            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;
            result = 1; //moze sa inicializovat pri definicii???
        }
        else if(token->type == token_return)
        {
            if(fn_expression(sourceFile, in_return) != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;
            result = 1;
        }
        else
            break;

    }while(0);

    return result;
}

int nontermStatementBody()
{
    int result = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_zlz)
        {
            if(nontermVnutroFunkcie() != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_zrz)
                break;
            result = 1;
        }
        else
        {
            get_back_token(token);
            if(nontermStatement() != 1)
                break;
            result = 1;
        }


    }while(0);

    return result;
}

int nontermElse()
{
    int result = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_else)
        {
            if(nontermStatementBody() != 1)
                break;
            result = 1;
            break;
        }
        else if(token->type == token_while || token->type == token_if || token->type == token_identifier || token->type == token_return || token->type == token_double || token->type == token_int || token->type == token_String_type)
        {
            get_back_token(token);
            result = 1;
        }
    }while(0);

    return result;
}

int nontermRozlisenieIdentifikatora()
{
    int result = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_sem)
        {
            result = 1;
        }
        else if(token->type == token_asg)
        {
            if(fn_expression(sourceFile, in_assign)  != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;

            result = 1;
        }
        else if(token->type == token_pal)
        {
            if(nontermFunctionsArguments() != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_par)
                break;
            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;
            result = 1;
        }
    }while(0);

    return result;
}

int nontermFunctionsArguments()
{
    int result = -1;

   /* get_token(token, sourceFile);

    do
    {
        if(token->type == token_par)
        {
            result = 1;
            get_back_token(token);
            break;
        }
        if(fn_expression(sourceFile, in_function) != 1)
            break;
        do
        {
            get_token(token, sourceFile);
             if(token->type == token_par)
             {
                result = 1;
                get_back_token(token);
                break;
             }

            if(token->type != token_com)
                break;
            if(fn_expression(sourceFile) != 1)
                break;

        }while(1);

    }while(0);
*/

        if(fn_expression(sourceFile, in_function) == 1) //vyhodnotia sa vsetky argumenty
            result = 1;

    return result;
}
