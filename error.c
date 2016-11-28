#include "error.h"

/*
 * Funkce volana pri chybe
 */
void error_f(int error_num)
{
	switch(error_num)
	{
		case ERROR_LEX:
				fprintf(stderr, "Error: lexical analysis error.\n");
				break;
		case ERROR_SYNTAX:
				fprintf(stderr, "Error: syntax error.\n");
				break;
		case ERROR_SEM_UNDEFINED:
				fprintf(stderr, "Error: undefined class/function/variable.\n");
				break;
		case ERROR_SEM_TYPE:
				fprintf(stderr, "Error: incompatible types.\n");
				break;
		case ERROR_SEM_OTHER:
				fprintf(stderr, "Error: semantic mistake.\n");
				break;
		case ERROR_INPUT:
				fprintf(stderr, "Error: error in loading numerical value from input.\n");
				break;
		case ERROR_UNINIT_VAR:
				fprintf(stderr, "Error: working with uninitialized variables.\n");
				break;
		case ERROR_DIV_ZERO:
				fprintf(stderr, "Error: division by zero.\n");
				break;
		case ERROR_OTHER:
				fprintf(stderr, "Error: unspecified error.\n");
				break;
		default:
				fprintf(stderr, "Error: internal error of interpreter.\n");
				break;
	}
	
	fclose(sourceFile);
	
	memory_manager_free_all(); // uvolneni pameti
	
	exit(error_num); // ukonceni s navratovou hodnotou chyby
}
