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

int globalVarsOffset;


///premenne pre funkcie
token_type actFunctionType;
int nParams; //pocet parametrov aktualnej funkcie
bool hasResult;
///premenne pre funkcie

int main( int argc, char *argv[])
{

    if(argc != 2)
    {
        fprintf(stderr,"Zly zadany vstup\n");
        error_f(ERROR_INPUT);
    }
     else
     {
        sourceFile=fopen(argv[1],"r");
        if(sourceFile==NULL)
        {
            fprintf(stderr,"Cant open file\n");
            error_f(ERROR_INPUT);
        }
     }

    firstScan = true;
    global_table = ht_init(100); //////ak to tam spadne, neuvolnim svoju pamet
    globalVarsOffset=0;

    sVariableGlobal = memory_manager_malloc(sizeof(VariableStack));
    sVariableLocal = memory_manager_malloc(sizeof(VariableStack));
    VStackInit(sVariableGlobal, 50);
    VStackInit(sVariableLocal, 50);

    ht_table_item_ptr runPointer; //zaciatocna funkcia

    T_instr_list * instructionList; //instrukcny list
    instructionList = memory_manager_malloc(sizeof(T_instr_list));
    listInit(instructionList); //inicializacia instrukcneho listu

    actTrieda = memory_manager_malloc(sizeof(String));
    strInit(actTrieda);

    T_address_code * code;

    token = memory_manager_malloc(sizeof(T_token));

    Init_token(token);

    nontermClass(instructionList);

    fseek(sourceFile,0,0);
    firstScan = false;

    nontermClass(instructionList);

    runPointer = ht_search("Main_run",true, global_table);
    if(runPointer == NULL)
    {
        fprintf(stderr, "Nenasla sa funkcia run\n");
        error_f(ERROR_SEM_UNDEFINED);
    }
    else
    {
        if(runPointer->func->type != token_void || runPointer->func->nParams != 0)
        {
            fprintf(stderr, "Zly printf alebo pocet parametrov\n");
            error_f(ERROR_SEM_UNDEFINED);
        }
        else //generujem zavolanie run
        {
            code = memory_manager_malloc(sizeof(T_address_code));
            code->address1 = runPointer->func->instructionList;
            T_address * adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=true;
            code->address2 = adresa;
            code->operation = T_FUNC;
            listInsert(instructionList, code);
            generator(instructionList, true); //zavolanie interpretu
        }
    }

    free_token(token);

    ht_free(global_table);
    fclose(sourceFile);
    memory_manager_free_all();
    return 0;
}
void nontermClass(T_instr_list * instructionList)
{
    get_token(token, sourceFile);

    if(token->type == token_class)
    {
        get_token(token, sourceFile);
        if(token->type != token_identifier)
            error_f(ERROR_SYNTAX);

        if(strcmp(token->value,"ifj16") == 0)
            error_f(ERROR_SEM_UNDEFINED);

        strLoad(actTrieda,token->value);
        get_token(token, sourceFile);

        if(token->type != token_zlz)
           error_f(ERROR_SYNTAX);

        nontermVnutroTriedy(instructionList);

        get_token(token, sourceFile);
        if(token->type != token_zrz)
           error_f(ERROR_SYNTAX);

        nontermClass(instructionList);

        if(token->type == token_EOF)
        {
            //OK
        }
    }
    else if(token->type == token_EOF)
    {
        //koniec
    }
    else
        error_f(ERROR_SYNTAX);
}

void nontermVnutroTriedy(T_instr_list * instructionList)
{
    String *key;
    T_instr_list * functionBodyList;
    ht_table_item_ptr tableItem;
    key = (String*) memory_manager_malloc(sizeof(String));
    strInit(key);

    do
    {
       get_token(token, sourceFile);
       if(token->type == token_zrz) //je tam koniec bloku
       {
            get_back_token(token);
            break;
       }
       if(token->type != token_static) //povinny static
            error_f(ERROR_SYNTAX);

       get_token(token, sourceFile);
       if(token->type == token_void) //osetrenie samostatnej vetvy ak je funkcia void
       {
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                error_f(ERROR_SYNTAX);

            strLoad(key, token->value);
            get_token(token, sourceFile);

           ///pripravenie identifikatora
                strLoad(key, actTrieda->str);
                strCat(key, "_");
                strCat(key, token->value);
            ///pripravenie identifikatora

            if(token->type == token_pal) //funkcia
            {
                actFunctionType = token_void;
                nParams = 0;
                if(!firstScan)
                {
                    actFunctionOffset=0;
                    local_table = ht_init(100); // pre kazdu funkciu jedna tabulka
                }

                nontermFunctionParams(key);

                get_token(token, sourceFile);
                if(token->type != token_zlz)
                    error_f(ERROR_SYNTAX);

                if(!firstScan)
                {
                    tableItem = memory_manager_malloc(sizeof(ht_table_item_ptr));
                    tableItem = ht_search(key->str, true, global_table);
                    if(tableItem == NULL || tableItem->func == NULL)
                    {
                        fprintf(stderr,"Nedefinovana funkcia\n");
                        error_f(ERROR_SEM_UNDEFINED);
                    }

                    functionBodyList = tableItem->func->instructionList;
                }

                nontermVnutroFunkcie(functionBodyList);

                if(!firstScan) //vymazanie lokalnej tabulky
                    ht_free(local_table);

                get_token(token, sourceFile);
                if(token->type != token_zrz)
                   error_f(ERROR_SYNTAX);

                nontermVnutroTriedy(instructionList);
                break;
            }
            else
                error_f(ERROR_SYNTAX);

       }
       if(token->type != token_double && token->type != token_int && token->type != token_String_type && token->type != token_boolean)
            error_f(ERROR_SYNTAX);

        token_type actType;
        actType = token->type;

        get_token(token, sourceFile);

        if(token->type != token_identifier)
            error_f(ERROR_SYNTAX);
        ///
            strLoad(key, actTrieda->str);
            strCat(key, "_");
            strCat(key, token->value);
        ///pripravenie identifikatora

        actFunctionType = actType;

        nontermTriednyClen(key, instructionList);
    }while(0);

}
void nontermTriednyClen(String *key, T_instr_list * instructionList)
{
    bool premennaDefinition=false;
    bool premennaInit=false;
    T_instr_list * functionBodyList;
    ht_table_item_ptr tableItem;
    get_token(token, sourceFile);
    T_address_code *code;
    T_address *adresa = NULL;
    T_variable * premenna;

    if(token->type == token_pal) //funkcia
    {
        nParams=0;
        if(!firstScan)
        {
            actFunctionOffset=0; //na nultom je navratova hodnota funkcie, nastavenie offsetov pre dalsie funkcie
            hasResult = false;
            local_table = ht_init(100); // pre kazdu funkciu jedna tabulka
        }

        nontermFunctionParams(key);

        get_token(token, sourceFile);
        if(token->type != token_zlz)
            error_f(ERROR_SYNTAX);

        if(!firstScan)
        {
            functionBodyList = memory_manager_malloc(sizeof(T_instr_list));
            tableItem = memory_manager_malloc(sizeof(ht_table_item_ptr));
            tableItem = ht_search(key->str, true, global_table);
            if(tableItem == NULL || tableItem->func == NULL)
            {
                fprintf(stderr,"Nedefinovana funkcia\n");
                error_f(ERROR_SEM_UNDEFINED);
            }

            functionBodyList = tableItem->func->instructionList;
        }

        nontermVnutroFunkcie(functionBodyList);

        if(!hasResult)
        {
            fprintf(stderr,"Chybny return \n");
            error_f(ERROR_SEM_OTHER);
        }

        if(!firstScan) //uvolnim lokalnu tabulku
            ht_free(local_table);

         get_token(token, sourceFile);
         if(token->type != token_zrz)
            error_f(ERROR_SYNTAX);

    }
    else if(token->type == token_asg) //premenna priradenie
    {
       premennaDefinition = true;
       premennaInit = true;

       ///zapis do stacku
        if(firstScan)
        {
            //pushnutie na prazdno
            code =  memory_manager_malloc(sizeof(T_address_code));
            premenna = memory_manager_malloc(sizeof(T_variable));

            if(actFunctionType == token_int)
            {
                premenna->type = INT;
                premenna->value.value_int = 0;
            }
            else if(actFunctionType == token_double)
            {
                premenna->type = DOUBLE;
                premenna->value.value_double = 0;
            }
            else if(actFunctionType == token_String_type)
            {
                premenna->type = STRING;
                premenna->value.value_String = NULL;
            }
            else
            {
                premenna->type = BOOLEAN;
                premenna->value.value_bool = 1;
            }

            code->address1 = premenna;
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=true; //s ktorym zasobnikom pracuje
            code->result = adresa;
            code->operation = T_PUSH;
            listInsert(instructionList, code);
            //pushnutie na prazdno
        }

        fn_expression(in_assign_first,actFunctionType, NULL, &instructionList);

        get_token(token, sourceFile);

        if(firstScan)
        {
            code =  memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));

            adresa->isGlobal=false;
            adresa->offset = 0; //na zaciatku lokalneho zasobnika
            code->address1 = adresa;

            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=true;
            adresa->offset = globalVarsOffset;
            code->address2 = adresa;
            code->operation = T_MOV;
            listInsert(instructionList, code);

            //popnem vyraz z lokalneho zasobnika
            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false;
            code->result = adresa;
            code->operation = T_POP;
            listInsert(instructionList, code);
        }

       ///zapis do stacku
        if(token->type != token_sem)
            error_f(ERROR_SYNTAX);
    }
    else if(token->type == token_sem) //len definicia -> pushnem naprazdno na zasobnik
    {
        premennaDefinition = true;
        if(firstScan)
        {
            code =  memory_manager_malloc(sizeof(T_address_code));
            premenna = memory_manager_malloc(sizeof(T_variable));
            if(actFunctionType == token_int)
            {
                premenna->type = INT;
                premenna->value.value_int = 0;
            }
            else if(actFunctionType == token_double)
            {
                premenna->type = DOUBLE;
                premenna->value.value_double = 0;
            }
            else if(actFunctionType == token_String_type)
            {
                premenna->type = STRING;
                premenna->value.value_String = NULL;
            }
            else
            {
                premenna->type = BOOLEAN;
                premenna->value.value_bool = 1;
            }

            code->address1 = premenna;
            code->address2 = NULL;
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=true; //s ktorym zasobnikom pracuje
            code->result = adresa;
            code->operation = T_PUSH;
            listInsert(instructionList, code);
        }
    }
    else
        error_f(ERROR_SYNTAX); ///nesom si isty

    ///zapis globalnej premennej do tabulky
    if(premennaDefinition && firstScan)
    {
        ht_item_var_ptr item = ht_create_item_var(actFunctionType,globalVarsOffset,premennaInit);
        if(ht_insert_item_var(item, key->str, global_table) == false)
        {
            fprintf(stderr,"Redeklaracia globalnej premennej\n");
            error_f(ERROR_SEM_UNDEFINED);
        }

         globalVarsOffset++;
    }
    ///zapis premennej do tabulky

    nontermVnutroTriedy(instructionList);
}

void nontermFunctionParams(String *key)
{
    ht_params_ptr ht_param = NULL;
    ht_params_ptr ht_next = NULL;
    ht_params_ptr ht_firstParam = NULL;
    token_type param_type;
    T_instr_list * instructionList;
    String *strParName;
    int paramsOffset = -1;

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_par)
            break;

        //prvy parameter

        if(token->type != token_double && token->type != token_int && token->type != token_String_type && token->type != token_boolean)
            error_f(ERROR_SYNTAX);

        param_type = token->type;

        get_token(token, sourceFile);
        if(token->type != token_identifier)
            error_f(ERROR_SYNTAX);

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
        nParams++;
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
                fprintf(stderr,"Redeklaracia lokalnej premennej\n");
                error_f(ERROR_SEM_UNDEFINED);
            }
        }
             paramsOffset--;
        ///zapis parametra do lokalnej tabulky


        //dalsie parametre
        do
        {
            get_token(token, sourceFile);
            if(token->type == token_par)
                break;

            if(token->type != token_com)
                error_f(ERROR_SYNTAX);
            get_token(token, sourceFile);
            if(token->type != token_double && token->type != token_int && token->type != token_boolean && token->type != token_String_type)
                error_f(ERROR_SYNTAX);
            param_type = token->type;
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                error_f(ERROR_SYNTAX);

            nParams++;

            //ulozenie dalsieho parametra do funkcie
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
                    fprintf(stderr,"Redeklaracia lokalnej premennej\n");
                    error_f(ERROR_SEM_UNDEFINED);
                }
            }
             paramsOffset--;
            ///zapis parametra do lokalnej tabulky


        }while(1);

    }while(0);

      ///zapis funkcie do globalnej tabulky
      if(firstScan)
      {
        instructionList = memory_manager_malloc(sizeof(T_instr_list)); //vytvorenie noveho instrukcneho listu pre funkciu
        listInit(instructionList);
        ht_item_func_ptr item = ht_create_item_func(actFunctionType,instructionList,ht_firstParam, nParams);
        if(ht_insert_item_func(item, key->str, global_table) == false)
        {
            fprintf(stderr,"Redeklaracia globalnej funkcie\n");
            error_f(ERROR_SEM_UNDEFINED);
        }
      }
}
void nontermVnutroFunkcie(T_instr_list * instructionList)
{
    get_token(token, sourceFile);

    do
    {
        if(token->type == token_zrz)
        {
            get_back_token(token);
            break;
        }
        else
            get_back_token(token);
        nontermStatement(instructionList);

        nontermVnutroFunkcie(instructionList);
    }while(0);
}
void nontermStatement(T_instr_list * instructionList)
{
    token_type actType;
    bool premennaInit=false;
    String *key;
    T_instr_list * tempInstrList = NULL; //pre if a while
    T_address_code *code;
    String *potencionalClass;
    bool inaTrieda;
    T_variable * premenna;
    T_address * adresa;
    ht_item_var_ptr item;

    if(!firstScan)
    {
        key = (String*) memory_manager_malloc(sizeof(String));
        code = memory_manager_malloc(sizeof(T_address_code));
        potencionalClass = (String*) memory_manager_malloc(sizeof(String));
        strInit(key);
        strInit(potencionalClass);
    }

    get_token(token, sourceFile);

    do
    {
        if(token->type == token_if)
        {
            get_token(token, sourceFile);
            if(token->type != token_pal)
                error_f(ERROR_SYNTAX);

            fn_expression(in_if,BOOLEAN,NULL,&tempInstrList);

            if(!firstScan)
                code->address1 = tempInstrList; //podmienka

            get_token(token, sourceFile);
            if(token->type != token_par)
                error_f(ERROR_SYNTAX);

            nontermStatementBody(&tempInstrList);

            if(!firstScan)
                code->address2 = tempInstrList; //telo ifu

            nontermElse(&tempInstrList);

            if(!firstScan)
                code->result = tempInstrList;

            if(!firstScan)
            {
                code->operation = T_IF;
                listInsert(instructionList, code);

                code = memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal=false;
            }
        }
        else if(token->type == token_while)
        {
            get_token(token, sourceFile);
            if(token->type != token_pal)
                error_f(ERROR_SYNTAX);

            fn_expression(in_while,BOOLEAN,NULL,&tempInstrList);


           if(!firstScan)
                code->address1 = tempInstrList; //podmienka

            get_token(token, sourceFile);
            if(token->type != token_par)
                error_f(ERROR_SYNTAX);

            nontermStatementBody(&tempInstrList);

             if(!firstScan)
             {
                code->address2 = tempInstrList; //podmienka
                code->operation = T_WHILE;
                listInsert(instructionList, code);
             }
        }
        ///vestavjene funkcie
        else if(isInFunction(instructionList))
        {
            break;
        }
        ///vestavjene funkcie
        else if(token->type == token_identifier)
        {
            if(!firstScan)
            {
                strLoad(potencionalClass,token->value); //potencionalna trieda
            }

            get_token(token, sourceFile);
            if(token->type == token_dot) //rozsireny vyraz
            {
                get_token(token, sourceFile);
                if(token->type != token_identifier)
                    error_f(ERROR_SYNTAX);

                 if(!firstScan)
                 {
                     strLoad(key,potencionalClass->str);
                     strCat(key,"_");
                     strCat(key,token->value);
                     inaTrieda = true;
                 }
            }
            else
            {
                 if(!firstScan)
                 {
                     strLoad(key,potencionalClass->str);
                     inaTrieda = false;
                 }

                 get_back_token(token);
            }

            nontermRozlisenieIdentifikatora(instructionList, key, inaTrieda);
        }
        else if(token->type == token_double || token->type == token_boolean || token->type == token_int || token->type == token_String_type)
        {
            actType = token->type;
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                error_f(ERROR_SYNTAX);

            //generovanie instrukcii
            if(!firstScan)
            {
                strLoad(key, token->value);
                code =  memory_manager_malloc(sizeof(T_address_code));
                premenna = memory_manager_malloc(sizeof(T_variable));
                if(actType == token_int)
                {
                    premenna->type = INT;
                    premenna->value.value_int = 0;
                }
                else if(actType == token_double)
                {
                    premenna->type = DOUBLE;
                    premenna->value.value_double = 0;
                }
                else if(actType == token_String_type)
                {
                    premenna->type = STRING;
                    premenna->value.value_String = NULL;
                }
                else
                {
                    premenna->type = BOOLEAN;
                    premenna->value.value_bool = true;
                }

                code->address1 = premenna;
                code->address2 = NULL;
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = false; //s ktorym zasobnikom pracuje
                code->result = adresa;
                code->operation = T_PUSH;
                listInsert(instructionList, code);

                ///zapis lokalnej premennej
                if(!firstScan)
                {
                    item = ht_create_item_var(actType,actFunctionOffset,premennaInit);
                    actFunctionOffset++;
                    if(ht_insert_item_var(item, key->str, local_table) == false)
                    {
                        fprintf(stderr,"Nedeklarovana premenna\n");
                        error_f(ERROR_SEM_UNDEFINED);
                    }
                }
            ///zapis lokalnej premennej
            }
            //generovanie instrukcii

            get_token(token, sourceFile);
            if(token->type == token_asg)
            {
                premennaInit = true;

                fn_expression(in_assign_second,actType,NULL, &instructionList);

                if(!firstScan)
                {
                    code =  memory_manager_malloc(sizeof(T_address_code));
                    adresa = memory_manager_malloc(sizeof(T_address));

                    adresa->isGlobal = false;
                    adresa->offset = actFunctionOffset;
                    code->address1 = adresa; //odkial
                    adresa = memory_manager_malloc(sizeof(T_address));
                    adresa->isGlobal = false;
                    adresa->offset = actFunctionOffset-1;
                    code->address2 = adresa; //kam
                    code->operation = T_MOV;

                    listInsert(instructionList, code);

                    adresa = memory_manager_malloc(sizeof(T_address));
                    adresa->isGlobal=false;

                    code = memory_manager_malloc(sizeof(T_address_code));
                    code->operation = T_POP;
                    code->result = adresa;
                    listInsert(instructionList, code);

                     item->inicialized = true;
                }
            }
            else
                get_back_token(token);

            get_token(token, sourceFile);
            if(token->type != token_sem)
                error_f(ERROR_SYNTAX);
        }
        else if(token->type == token_return)  ////////////////////////////nnei dokonceneee
        {
            get_token(token, sourceFile); //moznost bez vyrazu
            if(token->type == token_sem)
            {
                if(actFunctionType == token_void)
                   hasResult = true;
                else
                {
                    fprintf(stderr,"Chyba: Zly return\n");
                    error_f(ERROR_SEM_UNDEFINED);
                }
                break;
            }
            else
                get_back_token(token);

            fn_expression(in_return,actFunctionType,NULL, &instructionList); ///osetrit vooooid (moze mat return;) poslat do result type

            hasResult = true;

            if(!firstScan)
            {
                /// kontrola typov....doplniit!!

                code =  memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = false;
                adresa->offset = actFunctionOffset; //odkial
                code->address1 = adresa; //kam returnujem

                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = false;
                adresa->offset = -(nParams+1); //navratova hodnota je pod parametrami
                code->address2 = adresa;
                code->operation = T_MOV;
                listInsert(instructionList, code);

                adresa = memory_manager_malloc(sizeof(T_address));
                code = memory_manager_malloc(sizeof(T_address_code));
                adresa->isGlobal=false;
                code->operation = T_POP;
                code->result = adresa;
                listInsert(instructionList, code);

                code =  memory_manager_malloc(sizeof(T_address_code));
                code->operation = T_RETURN;
                listInsert(instructionList, code);
            }
            get_token(token, sourceFile);
            if(token->type != token_sem)
                error_f(ERROR_SYNTAX);
        }
        else
            error_f(ERROR_SYNTAX);
    }while(0);
}

void nontermStatementBody(T_instr_list ** instructionList)
{
    get_token(token, sourceFile);

    if(!firstScan)
    {
        *instructionList = memory_manager_malloc(sizeof(T_instr_list));
        listInit(*instructionList);
    }

    if(token->type == token_zlz) //zaciatok zlozeneho prikazu
    {
        nontermVnutroFunkcie(*instructionList);

        get_token(token, sourceFile);
        if(token->type != token_zrz)
            error_f(ERROR_SYNTAX);
    } //koniec zlozeneho prikazu
    else //jednoduchy prikaz
    {
        get_back_token(token);
        nontermStatement(*instructionList);
    }
}

void nontermElse(T_instr_list ** instructionList)
{
    get_token(token, sourceFile);

    if(token->type == token_else)
    {
        nontermStatementBody(instructionList);
    }
    else if(token->type == token_while || token->type == token_zrz || token->type == token_if || token->type == token_identifier || token->type == token_return || token->type == token_double || token->type == token_int || token->type == token_String_type || token->type == token_boolean)
    {
        get_back_token(token);
        *instructionList = NULL; //else nieje
    }

}

void nontermRozlisenieIdentifikatora(T_instr_list * instructionList, String* key, bool otherClass)
{
    ht_table_item_ptr tableItem;
    get_token(token, sourceFile);
    String *findVar;
    T_address_code *code;
    bool isGlobal;
    ht_item_func_ptr fnce = NULL;
    T_address * adresa;
    token_type actType;
    T_variable * premenna;

    if(!firstScan)
    {
        findVar = (String*) memory_manager_malloc(sizeof(String));
        tableItem = memory_manager_malloc(sizeof(struct ht_table_item));
        strInit(findVar);
    }

    do
    {
        if(token->type == token_sem)
            break;

        else if(token->type == token_asg)
        {
            ///odtestovanie ci taka premenna existuje
            if(!firstScan)
            {
                if(otherClass) //testovanie premennej z inej triedy
                {
                    tableItem = ht_search(key->str, false, global_table);
                    if(tableItem == NULL)
                    {
                        fprintf(stderr,"Takato globalna premenna neexistuje\n");
                        error_f(ERROR_SEM_UNDEFINED);
                    }
                    isGlobal = true;
                }
                else //testovanie z aktualnej triedy
                {
                     tableItem = ht_search(key->str, false, local_table);
                     isGlobal = false;
                     if(tableItem == NULL) //pozriem este na aktualnu triedu medzi globalne
                     {
                        strLoad(findVar, actTrieda->str);
                        strCat(findVar,"_");
                        strCat(findVar,key->str);
                        tableItem = ht_search(findVar->str, false, global_table);
                        if(tableItem == NULL)
                        {
                            fprintf(stderr,"Nedefinovana premenna\n");
                            error_f(ERROR_SEM_UNDEFINED);
                        }
                        isGlobal = true;
                     }
                     if(tableItem->var == NULL) //pripad kedy je to nazov funkcie, nie premennej
                     {
                         fprintf(stderr,"Nedefinovana premenna\n");
                         error_f(ERROR_SEM_UNDEFINED);
                     }
                }

            }

            ///odtestovanie ci taka premenna existuje ...............
            if(!firstScan)
                fn_expression(in_assign_second,tableItem->var->type,NULL, &instructionList);
            else
                fn_expression(in_assign_second,0,NULL, &instructionList);

            ///generovanie instrukcii
            if(!firstScan)
            {
                code = memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));

                adresa->isGlobal = false; //je na vrchole lokalneho stacku (vysledok po expression)
                adresa->offset = actFunctionOffset; /// !!!!!!! vrchol zasobnika
                code->address1 = adresa;

                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = isGlobal;
                adresa->offset = tableItem->var->offset;
                code->address2 = adresa;

                code->operation = T_MOV;
                listInsert(instructionList, code);
                tableItem->var->inicialized = true;
                //este zahodenie vysledku po expression

                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal=false;

                code = memory_manager_malloc(sizeof(T_address_code));
                code->operation = T_POP;
                code->result = adresa;
                listInsert(instructionList, code);
            }
            ///generovanie instrukcii

            get_token(token, sourceFile);
            if(token->type != token_sem)
               error_f(ERROR_SYNTAX);

            break;
        }
        else if(token->type == token_pal)
        {
            if(!firstScan) //najdenie funkcie v tabulke
            {
                fnce = memory_manager_malloc(sizeof(ht_item_func_ptr));
                if(otherClass) //testovanie premennej z inej triedy
                {
                    tableItem = ht_search(key->str, true, global_table);
                    if(tableItem == NULL)
                    {
                        fprintf(stderr,"Takato globalna funkcia neexistuje\n");
                        error_f(ERROR_SEM_UNDEFINED);
                    }
                    isGlobal = true;
                }
                else //testovanie z aktualnej triedy
                {
                     tableItem = ht_search(key->str, true, local_table);
                     isGlobal = false;
                     if(tableItem == NULL) //pozriem este na aktualnu triedu medzi globalne
                     {
                        strLoad(findVar, actTrieda->str);
                        strCat(findVar,"_");
                        strCat(findVar,key->str);
                        tableItem = ht_search(findVar->str, true, global_table);
                        if(tableItem == NULL)
                        {
                            fprintf(stderr,"Nedefinovana premenna\n");
                            error_f(ERROR_SEM_UNDEFINED);
                        }
                        isGlobal = true;
                     }
                     if(tableItem->func == NULL) //pripad kedy je to nazov premennej, nie funkcie
                     {
                         fprintf(stderr,"Nedefinovana funkcia\n");
                         error_f(ERROR_SEM_UNDEFINED);
                     }
                }
                fnce = tableItem->func;
            }

            ///generovanie instrukcii
             if(!firstScan)
            {
                //pushnutie naprazdno pre navratovu hodnotu
                code =  memory_manager_malloc(sizeof(T_address_code));
                premenna = memory_manager_malloc(sizeof(T_variable));
                actType = token_int; /////!!!!!!!!!!!!!!!vycucnut z tabulky
                if(actType == token_int)
                {
                    premenna->type = INT;
                    premenna->value.value_int = 0;
                }
                else if(actType == token_double)
                {
                    premenna->type = DOUBLE;
                    premenna->value.value_double = 0;
                }
                else if(actType == token_String_type)
                {
                    premenna->type = STRING;
                    premenna->value.value_String = NULL;
                }
                else
                {
                    premenna->type = BOOLEAN;
                    premenna->value.value_bool = true;
                }

                code->address1 = premenna;
                code->address2 = NULL;
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = false; //s ktorym zasobnikom pracuje
                code->result = adresa;
                code->operation = T_PUSH;
                listInsert(instructionList, code);
               //pushnutie naprazdno pre navratovu hodnotu

               actFunctionOffset++;
            }

           nontermFunctionsArguments(instructionList, fnce);

            if(!firstScan)
            {
                code = memory_manager_malloc(sizeof(T_address_code));

                code->address1 = fnce->instructionList;
                code->operation = T_FUNC;
                T_address * adresa = memory_manager_malloc(sizeof(T_address));

                if(actFunctionType == token_void)
                     adresa->isGlobal=true;
                else
                    adresa->isGlobal=false;

                code->address2 = adresa;
                listInsert(instructionList, code);

                //zahodenie vsetkych parametrov
                int x;
                for(x=0; x<fnce->nParams; x++)
                {
                    adresa = memory_manager_malloc(sizeof(T_address));
                    adresa->isGlobal = false;
                    code = memory_manager_malloc(sizeof(T_address_code));
                    code->operation = T_POP;
                    code->result = adresa;
                    listInsert(instructionList, code);
                }

                //zahodenie navratovej hodnoty ->vyuzivam len pri vyrazoch
                    code = memory_manager_malloc(sizeof(T_address_code));
                    adresa = memory_manager_malloc(sizeof(T_address));
                    adresa->isGlobal = false;
                    code->operation = T_POP;
                    code->result = adresa;
                    listInsert(instructionList, code);

                    actFunctionOffset--;
            }
            ///generovanie instrukcii
           /* get_token(token, sourceFile);
            if(token->type != token_par)
                break;*/ //expression skontroluje pravu zatvorku
            get_token(token, sourceFile);
            if(token->type != token_sem)
                error_f(ERROR_SYNTAX);
        }
    }while(0);
}

void nontermFunctionsArguments(T_instr_list * instructionList, ht_item_func_ptr fnce)
{
    get_back_token(token);
    fn_expression(in_function,0,fnce, &instructionList); //vyhodnotia sa vsetky argumenty
}

bool isInFunction(T_instr_list * instructionList) ////presuvat hodnotu na lokalnyyy
{
    T_address_code * code;
    T_address * adresa;

    if(token->type == token_iin)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
        if(token->type != token_par)
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
        if(token->type != token_sem)
           error_f(ERROR_SYNTAX);

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
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
        if(token->type != token_par)
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
        if(token->type != token_sem)
            error_f(ERROR_SYNTAX);

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
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
        if(token->type != token_par)
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
        if(token->type != token_sem)
           error_f(ERROR_SYNTAX);

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
           error_f(ERROR_SYNTAX);

        functPrintParams(instructionList);

        get_token(token, sourceFile);
        if(token->type != token_par)
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
         if(token->type != token_sem)
            error_f(ERROR_SYNTAX);

        return true;
    }
    else if(token->type == token_length || token->type == token_substr || token->type == token_compare || token->type == token_find || token->type == token_sort)
    {
        get_token(token, sourceFile);
        if(token->type != token_pal)
            error_f(ERROR_SYNTAX);

        get_back_token(token);

        if(token->type == token_length)
            fn_expression(in_length_fun,0,NULL,&instructionList);
        else if(token->type == token_substr)
            fn_expression(in_substr_fun,0,NULL,&instructionList);
        else if(token->type == token_compare)
           fn_expression(in_compare_fun,0,NULL,&instructionList);
        else if(token->type == token_find)
            fn_expression(in_find_fun,0,NULL,&instructionList);
        else if(token->type == token_sort)
            fn_expression(in_sort_fun,0,NULL,&instructionList);

        if(!firstScan) //nerobim s nou nic
        {
            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=false;
            code->result = adresa;
            code->operation = T_POP;
            listInsert(instructionList, code);
        }

        get_token(token, sourceFile);
        if(token->type != token_par)
            error_f(ERROR_SYNTAX);
        get_token(token, sourceFile);
         if(token->type != token_sem)
            error_f(ERROR_SYNTAX);

        return true;
    }

    return false; //nieje to vestavjena
}

void functPrintParams(T_instr_list * instructionList)
{
    T_address_code * code;
    T_address * adresa;
    T_variable * premenna;
    String *myString;
    int i=0;
    bool otherClass;
    ht_table_item_ptr tableItem;
    bool isGlobal;
    String *findVar;
    char* pEnd;

    do
    {
        get_token(token, sourceFile);
        if(token->type == token_String || token->type == token_number_double || token->type == token_number_int || token->type == token_true || token->type == token_false)
        {
            i++;
            if(!firstScan)
            {
                code =  memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));
                premenna = memory_manager_malloc(sizeof(T_variable));

                if(token->type  == token_number_int)
                {
                    premenna->type = INT;
                    premenna->value.value_int = atoi(token->value);
                }
                else if(token->type  == token_number_double )
                {
                    premenna->type = DOUBLE;
                    premenna->value.value_double = strtod(token->value, &pEnd);
                }
                 else if(token->type  == token_true )
                {
                    premenna->type = BOOLEAN;
                    premenna->value.value_bool = 1;
                }
                else if(token->type  == token_false )
                {
                    premenna->type = BOOLEAN;
                    premenna->value.value_bool = 0;
                }
                else if(token->type  == token_String)
                {
                    myString = memory_manager_malloc(sizeof(String));
                    strInit(myString);
                    strLoad(myString, token->value);
                    premenna->type = STRING;
                    premenna->value.value_String = myString;
                }

                code->address1 = premenna;
                adresa->isGlobal=false; //s ktorym zasobnikom pracuje
                code->result = adresa;
                code->operation = T_PUSH;
                listInsert(instructionList, code);

                code =  memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal=false;
                adresa->offset = actFunctionOffset;
                code->operation = T_OUT;
                code->address1 = adresa;
                listInsert(instructionList, code);

                code = memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal=false;
                code->result = adresa;
                code->operation = T_POP;
                listInsert(instructionList, code);
            }
        }
        else if(token->type == token_identifier)
        {
            i++;
            if(!firstScan)
            {
                tableItem = memory_manager_malloc(sizeof(struct ht_table_item));
                myString = memory_manager_malloc(sizeof(String));
                strInit(myString);
                strLoad(myString,token->value);
            }

            get_token(token, sourceFile);
            if(token->type == token_dot) //rozsireny vyraz
            {
                get_token(token, sourceFile);
                if(token->type != token_identifier)
                    error_f(ERROR_SYNTAX);

                if(!firstScan)
                {
                    strCat(myString,"_");
                     strCat(myString,token->value);
                     otherClass = true;
                }
            }
            else
            {
                get_back_token(token);
                otherClass = false;
            }

            if(!firstScan)
            {
                if(otherClass) //testovanie premennej z inej triedy
                {
                    tableItem = ht_search(myString->str, false, global_table);
                    if(tableItem == NULL)
                        error_f(ERROR_SYNTAX);

                    isGlobal = true;
                }
                else //testovanie z aktualnej triedy
                {
                     tableItem = ht_search(myString->str, false, local_table);
                     isGlobal = false;
                     if(tableItem == NULL) //pozriem este na aktualnu triedu medzi globalne
                     {
                        findVar = memory_manager_malloc(sizeof(String));
                        strInit(findVar);

                        strLoad(findVar, actTrieda->str);
                        strCat(findVar,"_");
                        strCat(findVar,myString->str);
                        tableItem = ht_search(findVar->str, false, global_table);
                        if(tableItem == NULL)
                        {
                            fprintf(stderr,"Nedefinovana premenna\n");
                            error_f(ERROR_SEM_UNDEFINED);
                        }

                        isGlobal = true;
                     }
                     if(tableItem->var == NULL) //pripad kedy je to nazov funkcie, nie premennej
                    {
                        fprintf(stderr,"Nedefinovana premenna\n");
                        error_f(ERROR_SEM_UNDEFINED);
                    }
                }

            code = memory_manager_malloc(sizeof(T_address_code));
            adresa = memory_manager_malloc(sizeof(T_address));
            adresa->isGlobal=isGlobal;
            adresa->offset = tableItem->var->offset;
            code->operation = T_OUT;
            code->address1 = adresa;
            listInsert(instructionList, code);
            }
        }
        else if(token->type == token_par)
        {
            get_back_token(token);
            break;
        }

        get_token(token, sourceFile);
        if(token->type == token_add)
            continue;
        else
        {
             get_back_token(token);
             break;
        }
    }while(1);

    if(i==0)
        error_f(ERROR_SEM_OTHER); ///overit

}
