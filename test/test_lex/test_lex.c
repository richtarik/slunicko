///* File: token.h             */
///* Autors: Milos Molitoris,  */
///* Login: xmolit00           */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 05.10.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../lex.h"

void f_print_type(T_token *tokenik);

int main()
{
    FILE* suborik= fopen("test_ostatni.txt","r");

    T_token tokenik;
    if(suborik == NULL)
    {
        printf("neotvoreny subor\n");
        return -1;
    }
    tokenik.value = malloc(sizeof(char) * ALLOC_BLOCK + 1); //rezerva pre znak koniec stringu
    if(tokenik.value == NULL)
    {
        fprintf(stderr,"Low memory\n");
        return(-2);
    }
    tokenik.valMaxsize = ALLOC_BLOCK; //set value maxSize
    tokenik.valActsize=0;


    while (tokenik.type != token_EOF && tokenik.type != token_error)
    {
        get_token(&tokenik,suborik);
        f_print_type(&tokenik);

    }

    fclose(suborik);
    return 0;
}

void f_print_type(T_token *tokenik)
{

        if(tokenik->type == token_String)
        {
            printf("String: %s\n",tokenik->value);
        }
        else if(tokenik->type == token_identifier)
        {
            printf("identifier: %s\n",tokenik->value);
        }
	else if(tokenik->type == token_class_identifier)
        {
            printf("class_identifier: %s\n",tokenik->value);
        }
        else if(tokenik->type == token_number_int)
        {
            printf("Int: %s\n",tokenik->value);
        }
        else if(tokenik->type == token_number_double)
        {
            printf("Double: %s\n",tokenik->value);
	    return;
        }
        else if(tokenik->type == token_EOF)
        {
            printf("koniec\n");
            return;
        }
	else if(tokenik->type == token_error)
        {
            printf("ERROR_token koniec programu\n");
	    return;
        }

        else
        {
		// 6 symbol slovo 1*\t 7symbol 2*\t
            switch(tokenik->type)
            {
                case 0:
                    printf("\"=\" \tAssignment \t type_num: %d\n",tokenik->type);
		    		break;
				case 1:
				    printf("\"+\" \tAddition \t type_num: %d\n",tokenik->type);
				    break;
				case 2:
				    printf("\"-\" \tSubtraction\t type_num: %d\n",tokenik->type);
				    break;
				case 3:
				    printf("\"*\" \tMultiplication \t type_num: %d\n",tokenik->type);
				    break;
				case 4:
				    printf("\"/\" \tDivision \t type_num: %d\n",tokenik->type);
				    break;
				case 5:
				    printf("\" \" \tModulo \t\t type_num: %d\n",tokenik->type);
				    break;
				case 6:
				    printf("\"++\" \tIncrement \t type_num: %d\n",tokenik->type);
				    break;
				case 7:
				    printf("\"--\" \tDecrement \t type_num: %d\n",tokenik->type);
				    break;
//Comparison operators/relational operators
				case 8:
				    printf("\"==\" \tEqual \t\t type_num: %d\n",tokenik->type);
				    break;
				case 9:
				    printf("\"!=\" \tNot equal \t type_num: %d\n",tokenik->type);
				    break;
				case 10:
				    printf("\">\" \tGreater \t type_num: %d\n",tokenik->type);
				    break;
				case 11:
				    printf("\"<\" \tLess \t\t type_num: %d\n",tokenik->type);
				    break;
				case 12:
				    printf("\">=\" \tGreater or equal type_num: %d\n",tokenik->type);
				    break;
				case 13:
				    printf("\"<=\" \tLess or equal \t type_num: %d\n",tokenik->type);
				    break;
//Logical operators
				case 14:
				    printf("\"||\" \tOr \t\t type_num: %d\n",tokenik->type);
				    break;
				case 15:
				    printf("\"&&\" \tAnd \t\t type_num: %d\n",tokenik->type);
				    break;
				case 16:
				    printf("\"!\" \tNot \t\t type_num: %d\n",tokenik->type);
				    break;
//other symbols
				case 17:
				    printf("\";\" \tSemicolon \t type_num: %d\n",tokenik->type);
				    break;
				case 18:
				    printf("\",\" \tColon \t\t type_num: %d\n",tokenik->type);
				    break;
				case 19:
				    printf("\".\" \tDot \t\t type_num: %d\n",tokenik->type);
				    break;
				case 20:
				    printf("\"(\" \tParenthes. left  type_num: %d\n",tokenik->type);
				    break;
				case 21:
				    printf("\")\" \tParenthes. right type_num: %d\n",tokenik->type);
				    break;
				case 22:
				    printf("\"[\" \tBracket left\t type_num: %d\n",tokenik->type);
				    break;
				case 23:
				    printf("\"]\" \tBracket right\t type_num: %d\n",tokenik->type);
				    break;


				case 24:
				    printf("\"{\" \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 25:
				    printf("\"}\" \t\t\t type_num: %d\n",tokenik->type);
				    break;

				case 26:
				    printf("break \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 27:
				    printf("continue \t\t type_num: %d\n",tokenik->type);
				    break;
				case 28:
				    printf("while \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 29:
				    printf("for \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 30:
				    printf("do \t\t\t type_num: %d\n",tokenik->type);
				    break;

				case 31:
				    printf("if \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 32:
				    printf("else \t\t\t type_num: %d\n",tokenik->type);
				    break;

				case 33:
				    printf("return \t\t\t type_num: %d\n",tokenik->type);
				    break;

				case 34:
				    printf("true \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 35:
				    printf("false \t\t\t type_num: %d\n",tokenik->type);
				    break;

				case 36:
				    printf("class \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 37:
				    printf("void \t\t\t type_num: %d\n",tokenik->type);
				    break;

				case 38:
				    printf("double \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 39:
				    printf("int \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 40:
				    printf("String \t\t\t type_num: %d\n",tokenik->type);
				    break;
				case 41:
				    printf("boolean \t\t type_num: %d\n",tokenik->type);
				    break;
				case 42:
				    printf("static \t\t\t type_num: %d\n",tokenik->type);
				    break;

				/* case xx:
				    printf("xxxx \t type_num: %d\n",tokenik->type);
				    break;
				*/

                default:
                  	printf("ERROR_DEAFAULT \t type_num: %d ----------------------------\n",tokenik->type);
		  			break;
            }
	    return;
        }
}
