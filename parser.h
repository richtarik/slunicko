#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "ial.h"

int nontermClass(T_instr_list * instructionList);
int nontermVnutroTriedy(T_instr_list * instructionList);
int nontermTriednyClen(String *key,  token_type actType, T_instr_list * instructionList);
int nontermFunctionParams(String *key, token_type actType);
int nontermVnutroFunkcie();
int nontermStatement(T_instr_list * instructionList);
int nontermStatementBody(T_instr_list * instructionList);
int nontermElse(T_instr_list * instructionList);
int nontermRozlisenieIdentifikatora(T_instr_list * instructionList, String* key, bool otherClass);
int nontermFunctionsArguments(T_instr_list * instructionList, ht_item_func_ptr fnce);

bool strToVal(String *retazec, value_type valType, union T_value * valStruct);

#endif
