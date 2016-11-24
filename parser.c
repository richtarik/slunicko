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

value_type * result_type; //aku hodnotu vracia fn_expression

int nParams; //pocet parametrov aktualnej funkcie

int main()
{
    firstScan = true;
    global_table = ht_init(100); //////ak to tam spadne, neuvolnim svoju pamet
    globalVarsOffset=1; //na nultej pozicii je miesto pre vysledok vestavjenej funkcie

    sVariableGlobal = memory_manager_malloc(sizeof(VariableStack));
    sVariableLocal = memory_manager_malloc(sizeof(VariableStack));
    VStackInit(sVariableGlobal, 50);
    VStackInit(sVariableLocal, 50);

    result_type = memory_manager_malloc(sizeof(value_type));

    //prazdny push pre vestavjene funkcie
    union T_value hodnota;
    hodnota.value_int = 0;
    VStackPush(sVariableGlobal,INT, hodnota);
    //prazdny push pre vestavjene funkcie
    ht_table_item_ptr runPointer; //zaciatocna funkcia

    T_instr_list * instructionList; //instrukcny list
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
        printf(stderr,"Cant open file");
        return -1;
   }

   Init_token(token);

   if(nontermClass(instructionList) == 1)
   {
       // printf("EVERYTHONG OK \n");
       print_table(global_table);
      // list_print(instructionList);
   }
   else
   {
		error_f(ERROR_SYNTAX);
   }
    fseek(sourceFile,0,0);
    firstScan = false;
    if(nontermClass(instructionList) == 1)
   {
        runPointer = ht_search("Main_run",true, global_table);
        if(runPointer == NULL)
            printf("Nenasla sa funkcia run\n");
        else
        {
            if(runPointer->func->type != token_void || runPointer->func->nParams != 0)
                printf("Zla definicia funkcie run\n");
            else
            {
                printf("SYNTAX OK \n");

                list_print(instructionList);
                generator(instructionList, true);
                print_VStack_data(sVariableGlobal);
            }
        }
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
int nontermClass(T_instr_list * instructionList)
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
            if(nontermVnutroTriedy(instructionList) != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_zrz)
                break;
              if(nontermClass(instructionList) != 1)
                break;
            if(token->type == token_EOF)
                result = 1;
        }
        else if(token->type == token_EOF)
            result = 1;
    }while(0);

    return result;

}

int nontermVnutroTriedy(T_instr_list * instructionList)
{
    int result = -1;
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
                    actFunctionOffset=0;
                    nParams = 0;
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

                if(!firstScan)
                {
                    tableItem = memory_manager_malloc(sizeof(ht_table_item_ptr));
                    tableItem = ht_search(key->str, true, global_table);
                    if(tableItem == NULL || tableItem->func == NULL)
                    {
                        printf("Nedefinovana funkcia\n");
                        break;
                    }

                    functionBodyList = tableItem->func->instructionList;
                }
                if(nontermVnutroFunkcie(functionBodyList) != 1)
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
                if(nontermVnutroTriedy(instructionList) != 1)
                    break;

                result = 1;
                break;
            }
            else
                break;

       }
       if(token->type != token_double && token->type != token_int && token->type != token_String_type && token->type != token_boolean)
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

        if(nontermTriednyClen(key, actType, instructionList) != 1)
            break;
        else
            result = 1;
    }while(0);

    return result;
}
int nontermTriednyClen(String *key,  token_type actType, T_instr_list * instructionList)
{
    int result = -1;
    bool premennaDefinition=false;
    bool premennaInit=false;
    T_instr_list * functionBodyList;
    ht_table_item_ptr tableItem;
    get_token(token, sourceFile);
    T_address_code *code;
    T_address *adresa = NULL;
    T_variable * premenna;
    do
    {
        if(token->type == token_pal) //funkcia
        {
            if(!firstScan)
            {
                actFunctionOffset=0; //na nultom je navratova hodnota funkcie, nastavenie offsetov pre dalsie funkcie
                nParams=0;
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

            if(!firstScan)
            {
                functionBodyList = memory_manager_malloc(sizeof(T_instr_list));
                tableItem = memory_manager_malloc(sizeof(ht_table_item_ptr));
                tableItem = ht_search(key->str, true, global_table);
                if(tableItem == NULL || tableItem->func == NULL)
                {
                    printf("Nedefinovana funkcia\n");
                    break;
                }

                functionBodyList = tableItem->func->instructionList;
            }

            if(nontermVnutroFunkcie(functionBodyList) != 1)
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
            if(fn_expression(in_assign,result_type,NULL, NULL) != 1)
                break;

           /* if(result_type == actType) ///treba dorobit konvertovanie typov
            {

            }
            else
                error_f(ERROR_SEM_TYPE);*/

            get_token(token, sourceFile);

           premennaDefinition = true;
           premennaInit = true;

           ///zapis do stacku
            if(firstScan)
            {
                //pushnutie na prazdno
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
                adresa->isGlobal=true; //s ktorym zasobnikom pracuje
                code->result = adresa;
                code->operation = T_PUSH;
                listInsert(instructionList, code);
                //pushnutie na prazdno

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
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal=false;
                code->result = adresa;

                code = memory_manager_malloc(sizeof(T_address_code));
                code->operation = T_POP;
                listInsert(instructionList, code);
            }
           ///zapis do stacku
            if(token->type != token_sem)
                break;
        }
        else if(token->type == token_sem) //len definicia -> pushnem naprazdno na zasobnik
        {
            premennaDefinition = true;
            if(firstScan)
            {
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
                adresa->isGlobal=true; //s ktorym zasobnikom pracuje
                code->result = adresa;
                code->operation = T_PUSH;
                listInsert(instructionList, code);
            }
        }
        else
            break;

        ///zapis globalnej premennej do tabulky
        if(premennaDefinition && firstScan)
        {
            ht_item_var_ptr item = ht_create_item_var(actType,globalVarsOffset,premennaInit);
            if(ht_insert_item_var(item, key->str, global_table) == false)
            {
                printf("Redeklaracia globalnej premennej\n");
                result = -1;
                break;
            }

             globalVarsOffset++;
        }
        ///zapis premennej do tabulky

        if(nontermVnutroTriedy(instructionList) != 1)
                break;
        result = 1;

    }while(0);

    return result;
}
int nontermFunctionParams(String *key,  token_type actType)
{
    int result = -1;
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
        {
            result = 1;
            break;
        }

        //prvy parameter

        if(token->type != token_double && token->type != token_int && token->type != token_String_type && token->type != token_boolean)
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
            nParams++;
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
            if(token->type != token_double && token->type != token_int && token->type != token_boolean && token->type != token_String_type)
                break;
            param_type = token->type;
            get_token(token, sourceFile);
            if(token->type != token_identifier)
                break;

            nParams++;

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

      ///zapis funkcie do globalnej tabulky
      if(firstScan)
      {
        instructionList = memory_manager_malloc(sizeof(T_instr_list)); //vytvorenie noveho instrukcneho listu pre funkciu
        ht_item_func_ptr item = ht_create_item_func(actType,instructionList,ht_firstParam, nParams);
        if(ht_insert_item_func(item, key->str, global_table) == false)
        {
            printf(stderr,"Redeklaracia globalnej funkcie");
            result = -1;
        }
      }
    ///zapis funkcie do globalnej tabulky
    return result;
}
int nontermVnutroFunkcie(T_instr_list * instructionList)
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
        if(nontermStatement(instructionList) != 1)
            break;
        if(nontermVnutroFunkcie(instructionList) != 1)
            break;
        result = 1;
    }while(0);
    return result;
}
int nontermStatement(T_instr_list * instructionList)
{
    int result = -1;
    token_type actType;
    bool premennaInit=false;
    String *key;
    T_instr_list  * tempInstrList = NULL; //pre if a while
    T_address_code *code;
    String *potencionalClass;
    bool inaTrieda;
    T_variable premenna;
    T_address * adresa;

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
                break;
            if(fn_expression(in_if,result_type,NULL,tempInstrList) != 1)
                break;

            if(!firstScan)
                code->address1 = tempInstrList; //podmienka

            get_token(token, sourceFile);
            if(token->type != token_par)
                break;
            if(nontermStatementBody(tempInstrList) != 1)
                break;

            if(!firstScan)
                code->address2 = tempInstrList; //telo ifu

            if(nontermElse(tempInstrList) != 1)
                break;

            if(!firstScan)
                code->result = tempInstrList;

            result = 1;

            if(!firstScan)
            {
                code->operation = T_IF;
                listInsert(instructionList, code);
            }
        }
        else if(token->type == token_while)
        {
            get_token(token, sourceFile);
            if(token->type != token_pal)
                break;
            if(fn_expression(in_while,result_type,NULL,tempInstrList) != 1)
                break;

           if(!firstScan)
                code->address1 = tempInstrList; //podmienka

            get_token(token, sourceFile);
            if(token->type != token_par)
                break;
            if(nontermStatementBody(tempInstrList) != 1)
                break;

             if(!firstScan)
                code->address1 = tempInstrList; //podmienka

            result = 1;
        }
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
                    break;

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

            if(nontermRozlisenieIdentifikatora(instructionList, key, inaTrieda) != 1)
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

            //generovanie instrukcii
            if(!firstScan)
            {
                code =  memory_manager_malloc(sizeof(T_address_code));

                if(actType == token_int)
                {
                    premenna.type = INT;
                    premenna.value.value_int = 0;
                }
                else if(actType == token_double)
                {
                    premenna.type = DOUBLE;
                    premenna.value.value_double = 0;
                }
                else if(actType == token_String_type)
                {
                    premenna.type = STRING;
                    premenna.value.value_String = NULL;
                }
                else
                {
                    premenna.type = BOOLEAN;
                    premenna.value.value_bool = true;
                }

                code->address1 = &premenna;
                code->address2 = NULL;
                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = true; //s ktorym zasobnikom pracuje
                code->result = adresa;
                code->operation = T_PUSH;
                listInsert(instructionList, code);
            }
            //generovanie instrukcii

            get_token(token, sourceFile);
            if(token->type == token_asg)
            {
                premennaInit = true;
                if(fn_expression(in_assign,result_type,NULL, NULL) != 1)
                    break;
                if(!firstScan)
                {
                    code =  memory_manager_malloc(sizeof(T_address_code));
                    adresa = memory_manager_malloc(sizeof(T_address));

                    adresa->isGlobal = false;
                    adresa->offset = actFunctionOffset;
                    code->address1 = adresa; //odkial
                    code->address2 = NULL; //kam

                    adresa = memory_manager_malloc(sizeof(T_address));
                    adresa->isGlobal=true;

                    adresa->offset = globalVarsOffset;
                    code->result = adresa;
                    code->operation = T_MOV;
                    listInsert(instructionList, code);
                }
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
            if(fn_expression(in_return,result_type,NULL, NULL) != 1)
                break;

            if(!firstScan)
            {
                code =  memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));

                adresa->isGlobal = false;
                adresa->offset = actFunctionOffset;
                code->address1 = adresa; //odkial

                adresa = memory_manager_malloc(sizeof(T_address));
                adresa->isGlobal = false;
                adresa->offset = -(nParams+1); //navratova hodnota je pod parametrami

                code->address2 = adresa; //kam
                code->operation = T_MOV;
                listInsert(instructionList, code);

                code->operation = T_RETURN;
                listInsert(instructionList, code);
            }

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

int nontermStatementBody(T_instr_list * instructionList)
{
    int result = -1;

    get_token(token, sourceFile);
    instructionList = memory_manager_malloc(sizeof(T_instr_list));
    do
    {
        if(token->type == token_zlz) //zaciatok zlozeneho prikazu
        {
            if(nontermVnutroFunkcie(instructionList) != 1)
                break;
            get_token(token, sourceFile);
            if(token->type != token_zrz)
                break;
            result = 1;
        } //koniec zlozeneho prikazu
        else //jednoduchy prikaz
        {
            get_back_token(token);
            if(nontermStatement(instructionList) != 1)
                break;
            result = 1;
        }


    }while(0);

    return result;
}

int nontermElse(T_instr_list * instructionList)
{
    int result = -1;
    get_token(token, sourceFile);

    do
    {
        if(token->type == token_else)
        {
            if(nontermStatementBody(instructionList) != 1)
                break;
            result = 1;
            break;
        }
        else if(token->type == token_while || token->type == token_if || token->type == token_identifier || token->type == token_return || token->type == token_double || token->type == token_int || token->type == token_String_type)
        {
            get_back_token(token);
            instructionList = NULL; //else nieje
            result = 1;
        }
    }while(0);

    return result;
}

int nontermRozlisenieIdentifikatora(T_instr_list * instructionList, String* key, bool otherClass)
{
    int result = -1;
    ht_table_item_ptr tableItem;
    get_token(token, sourceFile);
    String *findVar;
    T_address_code *code;
    bool isGlobal;
    ht_item_func_ptr fnce = NULL;
    T_address * adresa;

    if(!firstScan)
    {
        findVar = (String*) memory_manager_malloc(sizeof(String));
        tableItem = memory_manager_malloc(sizeof(struct ht_table_item));
        strInit(findVar);
    }

    do
    {
        if(token->type == token_sem)
        {
            result = 1;
        }
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
                        printf("Takato globalna premenna neexistuje\n");
                        break;
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
                            printf("Nedefinovana premenna\n");
                            break;
                        }
                        isGlobal = true;
                     }
                     if(tableItem->var == NULL) //pripad kedy je to nazov funkcie, nie premennej
                     {
                         printf("Nedefinovana premenna\n");
                         break;
                     }
                }

            }
            ///odtestovanie ci taka premenna existuje
            if(fn_expression(in_assign,result_type,NULL, NULL)  != 1)
                break;

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
                tableItem->var->inicialized = true;
                adresa->offset = tableItem->var->offset;

                code->operation = T_FUNC;
                listInsert(instructionList, code);

                //este zahodenie vysledku po expression
                code = memory_manager_malloc(sizeof(T_address_code));
                code->operation = T_POP;
                listInsert(instructionList, code);
            }
            ///generovanie instrukcii

            get_token(token, sourceFile);
            if(token->type != token_sem)
                break;

            result = 1;
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
                        printf("Takato globalna funkcia neexistuje\n");
                        break;
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
                            printf("Nedefinovana premenna\n");
                            break;
                        }
                        isGlobal = true;
                     }
                     if(tableItem->func == NULL) //pripad kedy je to nazov premennej, nie funkcie
                     {
                         printf("Nedefinovana funkcia\n");
                         break;
                     }
                }
                fnce = tableItem->func;
            }

            if(nontermFunctionsArguments(instructionList, fnce) != 1)
                break;

            ///generovanie instrukcii
             if(!firstScan)
            {
                code = memory_manager_malloc(sizeof(T_address_code));
                adresa = memory_manager_malloc(sizeof(T_address));

                adresa->isGlobal = false; //je na vrchole lokalneho stacku (vysledok po expression)
                adresa->offset = actFunctionOffset; /// !!!!!!! vrchol zasobnika
                code->address1 = fnce->instructionList;
                code->operation = T_FUNC;
                listInsert(instructionList, code);

                //zahodenie navratovej hodnoty
                        code = memory_manager_malloc(sizeof(T_address_code));
                        code->operation = T_POP;
                        listInsert(instructionList, code);

                //zahodenie vsetkych parametrov
                int x;
                for(x=0; x<fnce->nParams; x++)
                {
                    code = memory_manager_malloc(sizeof(T_address_code));
                    code->operation = T_POP;
                    listInsert(instructionList, code);
                }
            }
            ///generovanie instrukcii
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

int nontermFunctionsArguments(T_instr_list * instructionList, ht_item_func_ptr fnce)
{
    int result = -1;

    if(fn_expression(in_function,result_type,fnce, instructionList) == 1) //vyhodnotia sa vsetky argumenty
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
        valStruct->value_String = pomocnyRetazec->str; //neviem ci je to dobre

    }
    return result;
}
