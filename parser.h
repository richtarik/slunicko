#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "ial.h"

int nontermClass(T_instr_list * instructionList);
int nontermVnutroTriedy(T_instr_list * instructionList);
int nontermTriednyClen(String *key, T_instr_list * instructionList);
int nontermFunctionParams(String *key);
int nontermVnutroFunkcie();
int nontermStatement(T_instr_list * instructionList);
int nontermStatementBody(T_instr_list ** instructionList);
int nontermElse(T_instr_list ** instructionList);
int nontermRozlisenieIdentifikatora(T_instr_list * instructionList, String* key, bool otherClass);
int nontermFunctionsArguments(T_instr_list * instructionList, ht_item_func_ptr fnce);
bool isInFunction(T_instr_list * instructionList);

//bool strToVal(String *retazec, value_type valType, union T_value * valStruct);

#endif
