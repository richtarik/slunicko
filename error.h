#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>
#include <stdlib.h>

#include "memory_manager.h"

/*
 * Návratové hodnoty jednotlivých typů chyb
 */
enum errors {
	ERROR_LEX = 1,
	ERROR_SYNTAX = 2,
	ERROR_SEM_UNDEFINED = 3,
	ERROR_SEM_TYPE = 4,
	ERROR_SEM_OTHER = 6,
	ERROR_INPUT = 7,
	ERROR_UNINIT_VAR = 8,
	ERROR_DIV_ZERO = 9,
	ERROR_OTHER = 10,
	ERROR_INTERN = 99
};

void error_f(int error_num);

#endif
