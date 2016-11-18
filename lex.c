///* File: token.h             */
///* Autors: Milos Molitoris,  */
///*         Lukas Richtarik   */
///* Login: xmolit00, xricht25 */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 29.09.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "lex.h"

//TODO free
//TODO Token init and check reallock ... reallock all token no only data

/*TODO Prebytecné pocátecní císlice  0 v nenulovém císle vedou v rozšírení BASE na oktalovou reprezentaci celého císla.
*/
// ignotuje pociatecne 0 aj u celych cisel -> problem pri rozsireni na oktal. TODO repair
// OK Pro celou cást desetinného literálu i exponent platí, že prebytecné pocátecní císlice 0 jsou ignorovány. OK

T_token* token_buffer=NULL; // TODO = NULL ok ?

void get_back_token(T_token *token)
{
    token_buffer = token;
}
void free_token(T_token *token)
{
    token->value[0]='\0';
    memory_manager_free_one(token->value);
    token->valMaxsize=0;
    token->valActsize=0;
    memory_manager_free_one(token); 
}

bool Init_token(T_token *token)
{
    token->value = memory_manager_malloc(sizeof(char) * (base_token_size + 1) ); //rezerva pre znak koniec stringu
    if(token->value == NULL)
    {
		error_f(ERROR_LEX);
        //fprintf(stderr,"ERROR low memory first init\n");
        //free_token(token);
        //return false;
    }
    token->valMaxsize=base_token_size;
    token->valActsize=0;
    return true;
}

void get_token(T_token *token,FILE* sourceFile)
{
    lexikal_state state=state_Start;
    int actChar;
    int nty;
    token->valActsize=0;

    if(token_buffer != NULL)
    {
        token=token_buffer;
        token_buffer=NULL;
        return;
    }

    while(1)
    {
        actChar= fgetc(sourceFile);

        switch(state)
        {
            case state_Start:
                 if( ( isspace(actChar) ) )
                 {
                     break;
                 }
                 else if(isalpha(actChar) || actChar=='$' || actChar=='_')
                 {
                    f_addChar(actChar,token);
                    if( f_Identifier(sourceFile,token) )
                    {
                        f_findKeyW(token);
                        return;
                    }
                    else
                    {
                        token->type=token_error;
                        return;
                    }

                 }
                 else if(isdigit(actChar))
                 {
                     f_addChar(actChar,token);
                     state=state_Number;
                     break;
                 }
                 else if(actChar == EOF)
                 {
                     //TODO ok ?
                     token->type= token_EOF;
                     return;
                 }
                 else
                 {
                     switch (actChar) {
                     case '=':
                         state=state_Equal;
                         break;
                     case '+':
                         state=state_Addition;
                         break;
                     case '-':
                         state=state_Subtraction;
                         break;
                     case '*':
                         token->type= token_mul;
                         return;
                     case '/':
                         state=state_Division_or_comment;
                         break;
                     case '%':
                        token->type= token_mod;
                        return;

                     case '!':
                         state=state_Not;
                         break;
                     case '>':
                         state=state_Greater;
                         break;
                     case '<':
                         state=state_Less;
                         break;

                     case ';':
                         token->type= token_sem;
                         return;
                     case ',':
                         token->type= token_com;
                         return;
                     case '.':
			 actChar= fgetc(sourceFile);
			 if( isspace(actChar) )
			 {
				 error_f(ERROR_LEX);
			    //fprintf(stderr,"ERRor po bodke nesmie ist medzera\n"); //todo chybova hlaska;
                //free_token(token);
			    //token->type=token_error;
			 }
			 else
			 {
			     ungetc(actChar,sourceFile);
                             token->type= token_dot;
                         }
			 return;
                     case '"':
                          state=state_String;
                          break;

                     case '(':
                         token->type= token_pal;
                         return;
                     case ')':
                         token->type= token_par;
                         return;
                     case '[':
                         token->type= token_brl;
                         return;
                     case ']':
                         token->type= token_brr;
                         return;
                    case '{':
                         token->type= token_zlz;
                         return;
                    case '}':
                         token->type= token_zrz;
                         return;

                     case '|':
                         state=state_Or;
                         break;
                     case '&':
                         state=state_And;
                         break;
                     default:
						error_f(ERROR_LEX);
                         //free_token(token);
                         //token->type= token_error;
                         //return;
                         //TODO iny znak chyba
                         // break;
                     }
                 }
                 //new read - fgetc
                 break;

            case state_Equal:
                if(actChar == '=')
                {
                    token->type = token_equ;
                    return;
                }
                else
                {
                    token->type= token_asg;
                    ungetc(actChar,sourceFile);
                    return;
                }
            case state_Addition:
                if(actChar == '+')
                {
                    token->type = token_inc;
                    return;
                }
                else
                {
                    token->type= token_add;
                    ungetc(actChar,sourceFile);
                    return;
                }
            case state_Subtraction:
                if(actChar == '-')
                {
                    token->type= token_dec;
                    return;
                }
                else
                {
                    token->type= token_sub;
                    ungetc(actChar,sourceFile);
                    return;
                }
            case state_Division_or_comment:
                if (actChar == '*')
                {//Find " */ " or EOF-error
                    if(comment_in_block(sourceFile))
                    {// " */ "
                        state=state_Start;
                        break;
                    }
                    else
                    {// EOF
						error_f(ERROR_LEX);
                        //free_token(token);
                        //token->type=token_error;
                        //return;
                    }    //TODO ?? LEXX error - predcasny EOF //printf error
                }
                else if(actChar == '/')
                {// find new line or EOF
                   if( comment_in_line(sourceFile))
                   {//New line
                        state=state_Start;
                        break;
                   }
                   else
                   {//EOF //TODO OK ?? alebo error
                        token->type=token_EOF;
                        return;
                   }
                }
                else
                {
                    ungetc(actChar,sourceFile);
                    token->type= token_div;
                    return;
                }
                return;
            case state_Not:
                if(actChar == '=')
                {
                    token->type= token_neq;
                    return;
                }
                else
                {
                    token->type = token_not;
                    ungetc(actChar,sourceFile);
                    return;
                }
            case state_Greater:
                if(actChar== '=')
                {
                    token->type= token_goe;
                    return;
                }
                else
                {
                    token->type = token_gre;
                    ungetc(actChar,sourceFile);
                    return;
                }
            case state_Less:
                if(actChar == '=')
                {
                    token->type= token_loe;
                    return;
                }
                else
                {
                    token->type = token_les;
                    ungetc(actChar,sourceFile);
                    return;
                }
            case state_Or:
                if(actChar == '|')
                {
                    token->type= token_or;
                    return;
                }
                else
                {
                    ungetc(actChar,sourceFile); // ?? je to potreba ??
                    error_f(ERROR_LEX);
                    //free_token(token);
                    //token->type= token_error;
                    //fprintf(stderr, "ERROR nepovoleny znak | nieje ||\n");
                    //return;
                }
            case state_And:
                if(actChar == '&')
                {
                    token->type= token_and;
                    return;
                }
                else
                {
                    ungetc(actChar,sourceFile); // ?? je to potreba ??
                    error_f(ERROR_LEX);
                    //free_token(token);
                    //fprintf(stderr, "ERROR nepovoleny znak & nieje &&\n");
                    //token->type= token_error;
                    //TODO chyba alebo bitove operacie
                    //return;
                }

            case state_Number: //TODO pocitadlo cislic - rozdelovat int a double ??

                if( isdigit(actChar) )
                {
                    if(token->value[0] == '0')  //TODO vymaz to pre rozsirenie oktal inak ok
                        token->valActsize=0;

                    if(f_addChar(actChar,token))
                        break;
                    else //todo error low mem
                    {
                        token->type=token_error;
                        return;
                    }
                }
                else if (actChar == 'e' || actChar == 'E')
                {
                    state=state_Double_exp_1;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
                        token->type=token_error;
                        return;
                    }
                }
                else if ( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
					error_f(ERROR_LEX);
                    //free_token(token);
                    //fprintf(stderr, "ERROR znak nieje sucast cisla state_Number\n");
                    //token->type=token_error;
                    //return;
                }
                else if (actChar == '.')
                {
                    state=state_Double_dot;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
                        token->type=token_error;
                        return;
                    }
                }
                else
                {// iny -> nepovolenz znak v dalsom nacitany nastane chyba ak bude nejaky operator vsetko je ok
                    ungetc(actChar,sourceFile);
                    token->type=token_number_int;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
            case state_Double_dot:
                if( isdigit(actChar) )
                {
                    state=state_Double;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
                        token->type=token_error;
                        return;
                    }
                } // TODO skontroluj ci po bodke musi ist znak
                else if ( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
					error_f(ERROR_LEX);
                    //free_token(token);
                    //fprintf(stderr, "ERROR znak nieje sucast cisla state_Double_dot\n");
                    //token->type=token_error;
                    //return;

                }
                else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride iny znaky -> error
                    ungetc(actChar,sourceFile);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
            case state_Double:
                if( isdigit(actChar) )
                {
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
                        token->type=token_error;
                        return;
                    }
                }
                else if (actChar == 'e' || actChar == 'E')
                {
                    state=state_Double_exp_1;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
                        token->type=token_error;
                        return;
                    }
                }
                else if ( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
					error_f(ERROR_LEX);
                    //free_token(token);
                    //fprintf(stderr, "ERROR znak nieje sucast cisla state_Double\n");
                    //token->type=token_error;
                    //return;

                }
                else
                {  //ak pride operator ok je to cislo ale ak pride iny znaky -> error
                    ungetc(actChar,sourceFile);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
            case state_Double_exp_1:
	    // musi byt nejake cislo alebo znak inak chyba!!
		        nty=token->valActsize;
                if(isdigit(actChar) )
                {
                    state=state_Double_exp;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
						error_f(ERROR_LEX);
                        //token->type=token_error; //TODO low mem printf
                        //return;
                    }
                }
                else if (actChar == '+' || actChar == '-')
                {
                    state=state_Double_exp_sign;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
						error_f(ERROR_LEX);
                        //token->type=token_error;
                        //return;
                    }
                }
                else
                {
					error_f(ERROR_LEX);
                    //free_token(token);
                    //fprintf(stderr, "ERROR po znaku E/e musi ist cislo alebo znamienko state_Double_exp_1\n");
                    //token->type=token_error;
                    //return;
                }

            case state_Double_exp:
                if(isdigit(actChar) )
                {
                    state=state_Double_exp;
		            if(token->value[nty] == '0') //todo Ok? preskakuje 0 za e-/+
                        token->valActsize-=1;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
						error_f(ERROR_LEX);
                        //token->type=token_error;
                        //return;
                    }
                }
                else if ( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
					error_f(ERROR_LEX);
                    //free_token(token);
                    //fprintf(stderr, "ERROR nepovoleny znak state_Double_exp\n");
                    //return;
                }
                else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride iny znaky -> error
                    ungetc(actChar,sourceFile);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
            case state_Double_exp_sign:
		        nty++;
                if(isdigit(actChar) )
                {
                    state=state_Double_exp;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
						error_f(ERROR_LEX);
                        //token->type=token_error;
                        //return;
                    }
                }
                else // if( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
					error_f(ERROR_LEX);
		            //free_token(token);
                    //fprintf(stderr, "ERROR po znaku +/- musi ist cislo state_Double_exp_sign\n");
                    //token->type=token_error;
                    //return;

                }
                /*else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride abcd alebo ostatne znaky -> error
                    ungetc(actChar,sourceFile);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }*/
            case state_String:
                //f_addChar(actChar,token);
                ungetc(actChar,sourceFile);
                if ( check_String(sourceFile,token) )
                {
                    token->type=token_String;
                    return;
                }
                else
                {
					error_f(ERROR_LEX);
                    //todo chyba err printf mem or bad string
                    //token->type=token_error;
                    //return;
                }

        }
    }
    return;
}


bool comment_in_block (FILE* sourceFile)
{
    int c_next;
    while( ( c_next=fgetc(sourceFile) ) ) // TODO check EOF
    {
        if(c_next=='*')
        {
            if( (c_next=fgetc(sourceFile)) == '/')
            {
                return true; //koniec komentara
            }
            else
                continue;
        }
        else if(c_next==EOF) //TODO printf chybovy stav
        {
			error_f(ERROR_LEX);
            //fprintf(stderr, "ERROR neukonceny blokovy komentar\n");
            //return false;
        }
        else
            continue;
    }
    //c_next==EOF
    return false;
    //LEX_ERROR - predcasny koniec
}

bool comment_in_line(FILE* sourceFile)
{
    int c_next;
    while( (c_next=fgetc(sourceFile)) != EOF )
    {
        if(c_next=='\n')
        {
            return true;
        }
        else
            continue;
    }
    // EOF
    return true;
}

bool check_String(FILE* sourceFile, T_token *actToken)
{
    int actChar;
    while( ( actChar=fgetc(sourceFile) ) )
    {
        if(actChar == '"')
            break; //nasiel sa koniec stringu
        else if(actChar == '\\')
        {
            check_escape(sourceFile,actToken);
            continue;
        }
        else if(actChar == EOF || actChar == '\n') //nieje to string
	    {
			error_f(ERROR_LEX);
            //free_token(actToken);
            //fprintf(stderr,"ERROR: -> neukonceny string\n");
            //return false; //TODO pritf error chybovy stav -> warning
	    }
	    else
        {
            if(f_addChar(actChar,actToken))
                ;
            else
                return false; // TODO error printf mem -> free
        }
    }

    actToken->value[actToken->valActsize] = '\0';
    actToken->valActsize=0;
    return true;
}

void check_escape(FILE* sourceFile, T_token* actToken)
{//TODO '\ddd'
     //'\"' '\n'  '\t'  '\\'  '\ddd'
     int nextChar;
     nextChar=fgetc(sourceFile);
     int convert_result=0;
     char substring[2];

     switch(nextChar)
     {
        case 'n' :
            f_addChar('\n',actToken);
            return;
        case 't' :
            f_addChar('\t',actToken);
            return;
        case '"' :
            f_addChar('\"',actToken);
            return;
        case '\\' :
            f_addChar('\\',actToken);
            return;
        default:
            if( (nextChar) >='0' && (nextChar) < '9' )
            {
		        substring[0]=nextChar;
                convert_result=(nextChar-48)*8*8; // x*(8^2)
                nextChar=fgetc(sourceFile);
                if( (nextChar) >= '0' && (nextChar) < '9' )
                {
		            substring[1]=nextChar;
                    convert_result+=(nextChar-48)*8; // x*(8^1)
                    nextChar=fgetc(sourceFile);
                    if( (nextChar) >= '0' && (nextChar) < '9' )
                    {
                        convert_result+=nextChar-48; //x* 8^0== x*1
                        f_addChar( convert_result ,actToken);
                    }
                    else
                    {
			            f_addChar('\\',actToken);
                        f_addChar(substring[0],actToken);
                        f_addChar(substring[1],actToken);
                        f_addChar(nextChar,actToken);
                    }
                }
                else
                {
                    f_addChar('\\',actToken);
                    f_addChar(substring[0],actToken);
                    f_addChar(nextChar,actToken);
                }
            }
            else
            {
                f_addChar('\\',actToken);
                f_addChar(nextChar,actToken);
            }
            return;
     }

}

bool f_addChar (char symbol, T_token* token)
{
    if(token->valActsize == token->valMaxsize) // TODO ked pridavam posledny znak /0 zmesti sa ??
    {
        int newSize = sizeof(char) * (ALLOC_BLOCK + token->valMaxsize);
        token->value = realloc(token->value, newSize); //zvacsenie bloku pre string v tokene
        if(token->value == NULL)
        {
			error_f(ERROR_LEX);
            //fprintf(stderr,"Low memory -> Reallock -> add_char\n");
            //free_token(token);
            //return false;
        }
        token->valMaxsize += ALLOC_BLOCK;
    }
    token->value[token->valActsize]= symbol;
    token->valActsize+=1;
    return true;
}

bool f_Identifier(FILE* sourceFile, T_token *actToken)
{
    int actChar;
    while( ( actChar= fgetc(sourceFile) ) )
    {
        if( isalpha(actChar) || isdigit(actChar) || actChar=='$' || actChar=='_')
        {
            if(f_addChar(actChar,actToken))
                ;
            else
                return false;
        }
	    else if( actChar == '.')
        {
	    actToken->type=token_class_identifier;
	    ungetc(actChar,sourceFile);
            break;
	}
        else
        {
	        actToken->type=token_identifier;
            ungetc(actChar,sourceFile);
            break; //TODOO error bad symbol (pritf actChar)-> return false ->free mem
        }

    }
    actToken->value[actToken->valActsize] = '\0';
    actToken->valActsize=0;
    return true;
}

