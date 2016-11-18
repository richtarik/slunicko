#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "str.h"

int nontermClass();
int nontermVnutroTriedy();
int nontermTriednyClen(String *key,  token_type actType);
int nontermFunctionParams(String *key, token_type actType);
int nontermVnutroFunkcie();
int nontermStatement();
int nontermStatementBody();
int nontermElse();
int nontermRozlisenieIdentifikatora();
int nontermFunctionsArguments();

#endif
