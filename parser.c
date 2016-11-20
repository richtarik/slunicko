#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

#include "parser.h"
#include "lex.h"
#include "expr.h"
#include "ial.h"
#include "print_stack.h"
#include "string.h"
#include <limits.h>
#include "generator.h"

hash_table_ptr global_table;
hash_table_ptr local_table;
String * actTrieda; //v akej triede som aktualne

int globalVarsOffset;
int actFunctionOffset;
bool firstScan; //prepinac ci som v prvom alebo druhom zostupe

T_instr_list * instructionList;

int main()
{
    firstScan = true;
    global_table = ht_init(100); //////ak to tam spadne, neuvolnim svoju pamet
    globalVarsOffset=1; //na nultej pozicii je miesto pre vysledok vestavjenej funkcie

    sVariableGlobal = memory_manager_malloc(sizeof(VariableStack));
    sVariableLocal = memory_manager_malloc(sizeof(VariableStack));
    VStackInit(sVariableGlobal, 50);
    VStackInit(sVariableLocal, 50);

    //prazdny push pre vestavjene funkcie
    union T_value hodnota;
    hodnota.value_int = 0;
    VStackPush(sVariableGlobal,INT, hodnota);
    //prazdny push pre vestavjene funkcie

    instructionList = memory_manager_malloc(sizeof(T_instr_list));
    listInit(instructionList); //inicializacia instrukcneho listu

    actTrieda = memory_manager_malloc(sizeof(String));
    strInit(actTrieda);

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
       // printf("EVERYTHONG OK \n");
       //print_table(global_table);
   }
   else
   {
		error_f(ERROR_SYNTAX);
   }
    fseek(sourceFile,0,0);
    firstScan = false;
    if(nontermClass(sourceFile) == 1)
   {
        printf("EVERYTHONG OK \n");
        list_print(instructionList);
        print_VStack_data(globalVarsOffset);
   }
   else
   {
		error_f(ERROR_SYNTAX);
   }

    free_token(token);

    ht_free(global_table);
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

             ///
                strLoad(key, actTrieda->str);
                strCat(key, "_");
                strCat(key, token->value);
            ///pripravenie identifikatora

            if(token->type == token_pal) //funkcia
            {
                if(!firstScan)
                {
                    actFunctionOffset=1; //na nultom je navratova hodnota funkcie, nastavenie offsetov pre dalsie funkcie
                    local_table = ht_init(100); // pre kazdu funkciu jedna tabulka
                }

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
                if(!firstScan)
                {
                  //  printf("Lokalnaaaa\n");
                 //  print_table(local_table);
                  //  printf("/Lokalnaaaa\n");
                    ht_free(local_table);
                }
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
            if(!firstScan)
            {
                actFunctionOffset=1; //na nultom je navratova hodnota funkcie, nastavenie offsetov pre dalsie funkcie
                local_table = ht_init(100); // pre kazdu funkciu jedna tabulka
            }
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
            if(!firstScan)
            {
             //   printf("Lokalnaaaa\n");
             //   print_table(local_table);
             //   printf("/Lokalnaaaa\n");
                ht_free(local_table);
            }

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

           ///zapis do stacku
            if(firstScan)
            {
                T_address_code *code;
                T_address *adresa = NULL;
                code =  memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));

                code->address1 = NULL;
                code->address1 = NULL;
                adresa->isGlobal=true;
                adresa->offset = globalVarsOffset;
                code->result = adresa;
                code->operation = T_POP;
                listInsert(instructionList, code);
            }
           ///zapis do stacku
        }
        else if(token->type == token_sem)
        {
            premennaDefinition = true;
        }
        else
            break;

        ///zapis globalnej premennej do tabulky
        if(premennaDefinition && firstScan)
        {
            ht_item_var_ptr item = ht_create_item_var(actType,globalVarsOffset,premennaInit);
            globalVarsOffset++;
            if(ht_insert_item_var(item, key->str, global_table) == false)
            {
                printf("Redeklaracia globalnej premennej\n");
                result = -1;
                break;
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

    String *strParName;

    int paramsOffset = 0;

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

        //ulozenie prveho parametra do tabulky funkcie
        if(firstScan)
        {
            ht_param = (ht_params_ptr) memory_manager_malloc(sizeof(struct ht_params));
            ht_param->name = (char*) memory_manager_malloc(sizeof(char)*(token->valActsize)+1);

            strcpy(ht_param->name,token->value);

            ht_param->offset = paramsOffset;
            ht_param->type = param_type;
            ht_param->next=NULL;

            ht_firstParam = ht_param;
        }
        //ulozenie prveho parametra do tabulky funkcie

        ///zapis parametra do lokalnej tabulky
        if(!firstScan)
        {
            strParName = (String*) memory_manager_malloc(sizeof(String));
            strInit(strParName);
            strLoad(strParName,token->value);

            ht_item_var_ptr item = ht_create_item_var(param_type,paramsOffset,true);
            if(ht_insert_item_var(item, strParName->str, local_table) == false)
            {
                printf("Redeklaracia lokalnej premennej");
                result = -1;
            }
        }
             paramsOffset--;
        ///zapis parametra do lokalnej tabulky


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

            //ulozenie dalsieho parametra do tabulky
            if(firstScan)
            {
                ht_next = (ht_params_ptr) memory_manager_malloc(sizeof(struct ht_params));
                ht_next->name = (char*) memory_manager_malloc(sizeof(char)*(token->valActsize));
                (ht_next->name)[0] = '\0';
                strcpy(ht_next->name,token->value);

                ht_next->offset = paramsOffset;
                ht_next->type = param_type;
                ht_next->next=NULL;

                ht_param->next = ht_next;
                ht_param = ht_next;
            }
            //ulozenie prveho  parametra
             ///zapis parametra do lokalnej tabulky
            if(!firstScan)
            {
                strParName = (String*) memory_manager_malloc(sizeof(String));
                strInit(strParName);
                strLoad(strParName,token->value);

                ht_item_var_ptr item = ht_create_item_var(param_type,paramsOffset,true);
                if(ht_insert_item_var(item, strParName->str, local_table) == false)
                {
                    printf("Redeklaracia lokalnej premennej");
                    result = -1;
                }
            }
             paramsOffset--;
            ///zapis parametra do lokalnej tabulky


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

      ///zapis funkcie do globalnej tabulky
      if(firstScan)
      {
        ht_item_func_ptr item = ht_create_item_func(actType,listItemPtr,ht_firstParam);
        if(ht_insert_item_func(item, functionName->str, global_table) == false)
        {
            printf(stderr,"Redeklaracia globalnej funkcie");
            result = -1;
        }
      }
    ///zapis funkcie do globalnej tabulky
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
    token_type actType;
    bool premennaInit=false;
    String *key;

    key = (String*) memory_manager_malloc(sizeof(String));
    strInit(key);

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
            actType = token->type;
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                break;

            strLoad(key, token->value);

            get_token(token, sourceFile);
            if(token->type == token_asg)
            {
                premennaInit = true;
                if(fn_expression(sourceFile, in_assign) != 1)
                    break;
            }
            else
                get_back_token(token);

            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;
            result = 1;

            ///zapis lokalnej premennej
            if(!firstScan)
            {
                ht_item_var_ptr item = ht_create_item_var(actType,actFunctionOffset,premennaInit);
                actFunctionOffset++;
                if(ht_insert_item_var(item, key->str, local_table) == false)
                {
                    printf("Redeklaracia lokalnej premennej");
                    result = -1;
                }
            }
            ///zapis lokalnej premennej
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

bool strToVal(String *retazec, value_type valType, union T_value * valStruct)
{
    bool result = true;
    long i;

    if(valType == INT)
    {
       char *ptr; //ak chcem overit platnost cisla (to kontroluje uz lexikalka)

       i = strtol(retazec->str, &ptr, 10);
       if(i<(-INT_MAX) || i>INT_MAX)
            result = false; //pretecenie
        else
            valStruct->value_int = i;
    }
    else if(valType == DOUBLE) //pretecenie nerobim..bo neznam
    {
        double x;
        char *ptr;
        x = strtod(retazec->str, &ptr);
        valStruct->value_double = x;
    }
    else if(valType == BOOLEAN)
    {
        if(retazec->length==1 && retazec->str[0] == '0')
            valStruct->value_bool = 0;
        else if(retazec->length==1 && retazec->str[0] == '1')
            valStruct->value_bool = 1;
        else if(strcmp(retazec->str, "true")) //velke pismena mozme doriesit potom
            valStruct->value_bool = 1;
         else if(strcmp(retazec->str, "false")) //velke pismena mozme doriesit potom
            valStruct->value_bool = 0;
        else
            result = false;
    }
    else
    {
        String * pomocnyRetazec;
        pomocnyRetazec = memory_manager_malloc(sizeof(String));
        strInit(pomocnyRetazec);
        strLoad(pomocnyRetazec,retazec->str);
        valStruct->value_String = pomocnyRetazec; //neviem ci je to dobre

    }
    return result;
}
