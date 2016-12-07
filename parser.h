#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "ial.h"

void nontermClass(T_instr_list * instructionList);
void nontermVnutroTriedy(T_instr_list * instructionList);
void nontermTriednyClen(String *key, T_instr_list * instructionList);
void nontermFunctionParams(String *key);
void nontermVnutroFunkcie();
void nontermStatement(T_instr_list * instructionList);
void nontermStatementBody(T_instr_list ** instructionList);
void nontermElse(T_instr_list ** instructionList);
void nontermRozlisenieIdentifikatora(T_instr_list * instructionList, String* key, bool otherClass);
void nontermFunctionsArguments(T_instr_list * instructionList, ht_item_func_ptr fnce);
bool isInFunction(T_instr_list * instructionList);
void functPrintParams(T_instr_list * instructionList);
void functLengthParams(T_instr_list * instructionList);
//bool strToVal(String *retazec, value_type valType, union T_value * valStruct);

#endif
