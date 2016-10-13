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

//TODO string biele znaky /n /"->ok  ->-> \ddd sekvence OK
//TODO number prerusi  nacitavanie vsetko -> riesenie isalpha -> error _ $ -> OK
//TODO free

/*TODO Prebytecné pocátecní císlice  0 v nenulovém císle vedou v rozšírení BASE na oktalovou reprezentaci celého císla.
*/
// ignotuje pociatecne 0 aj u celych cisel -> problem pri rozsireni na oktal. TODO repair
// OK Pro celou cást desetinného literálu i exponent platí, že prebytecné pocátecní císlice 0 jsou ignorovány. OK


void get_token(T_token *token,FILE* filename)
{
    lexikal_state state=state_Start;
    int actChar;
    int nty;

    while(1)
    {
        actChar= fgetc(filename);

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
                    if( f_Identifier(filename,token) )
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
                         token->type= token_col;
                         return;
                     case '.':
			 actChar= fgetc(filename);
			 if( isspace(actChar) )
			 {
			     printf("ERRor po bodke nesmie ist medzera\n"); //todo chybova hlaska;
			     token->type=token_error;
			 }
			 else
			 {
			     ungetc(actChar,filename);
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
                         token->type= token_error;
                         return;
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
                    ungetc(actChar,filename);
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
                    ungetc(actChar,filename);
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
                    ungetc(actChar,filename);
                    return;
                }
            case state_Division_or_comment:
                if (actChar == '*')
                {//Find " */ " or EOF-error
                    if(comment_in_block(filename))
                    {// " */ "
                        state=state_Start;
                        break;
                    }
                    else
                    {// EOF
                        token->type=token_error;
                        return;
                    }    //TODO ?? LEXX error - predcasny EOF //printf error
                }
                else if(actChar == '/')
                {// find new line or EOF
                   if( comment_in_line(filename))
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
                    ungetc(actChar,filename);
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
                    ungetc(actChar,filename);
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
                    ungetc(actChar,filename);
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
                    ungetc(actChar,filename);
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
                    ungetc(actChar,filename);
                    token->type= token_error;
                    //TODO chyba alebo bitove operacie ?
                    return;
                }
            case state_And:
                if(actChar == '&')
                {
                    token->type= token_and;
                    return;
                }
                else
                {
                    ungetc(actChar,filename);
                    token->type= token_error;
                    //TODO chyba alebo bitove operacie
                    return;
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
                    printf("Error bad Number_state_int\n"); //TODO dopisat chybovu hlasku
                    token->type=token_error;
                    return;
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
                {// iny -> nepovolenz znak v dalsom nacitany nastane chyba
                    ungetc(actChar,filename);
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
                    printf("Error bad Number_state_Double_dot\n"); //TODO dopisat chybovu hlasku
                    token->type=token_error;
                    return;

                }
                else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride abcd alebo ostatne znaky -> error
                    ungetc(actChar,filename);
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
                    printf("Error bad Number_state_double\n"); //TODO dopisat chybovu hlasku
                    token->type=token_error;
                    return;

                }
                else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride abcd alebo ostatne znaky -> error
                    ungetc(actChar,filename);
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
                        token->type=token_error; //TODO low mem printf
                        return;
                    }
                }
                else if (actChar == '+' || actChar == '-')
                {
                    state=state_Double_exp_sign;
                    if(f_addChar(actChar,token))
                        break;
                    else
                    {
                        token->type=token_error;
                        return;
                    }
                }
                else
                {
                    printf("Error bad Number_state_double_e  po E musi ist cislo\n"); //TODO dopisat chybovu hlasku
                    token->type=token_error;
                    return;
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
                        token->type=token_error; //TODO low mem printf
                        return;
                    }
                }
                else if ( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
                    printf("Error bad Number_state_double_e  po E musi ist cislo\n"); //TODO dopisat chybovu hlasku
                    token->type=token_error;
                    return;
                }
                else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride abcd alebo ostatne znaky -> error
                    ungetc(actChar,filename);
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
                        token->type=token_error;
                        return;
                    }
                }
                else // if( isalpha(actChar) || actChar == '$'|| actChar == '_' )
                {
                    printf("Error bad Number_state_double_sign ->za znamienkom musi ist cislo\n"); //TODO dopisat chybovu hlasku
                    token->type=token_error;
                    return;

                }
                /*else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride abcd alebo ostatne znaky -> error
                    ungetc(actChar,filename);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }*/
            case state_String:
                //f_addChar(actChar,token);
                ungetc(actChar,filename);
                if ( check_String(filename,token) )
                {
                    token->type=token_String;
                    return;
                }
                else
                {
                    //todo chyba err printf mem or bad string
                    token->type=token_error;
                    return;
                }

        }
    }
    return;
}


bool comment_in_block (FILE* filename)
{
    int c_next;
    while( ( c_next=fgetc(filename) ) ) // TODO check EOF
    {
        if(c_next=='*')
        {
            if( (c_next=fgetc(filename)) == '/')
            {
                return true; //koniec komentara
            }
            else
                continue;
        }
        else if(c_next==EOF) //TODO printf chybovy stav
            return false;
        else
            continue;
    }
    //c_next==EOF
    return false;
    //LEX_ERROR - predcasny koniec
}

bool comment_in_line(FILE* filename)
{
    int c_next;
    while( (c_next=fgetc(filename)) != EOF )
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
            return false; //TODO pritf error chybovy stav
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
            fprintf(stderr,"Low memory\n");
            return false;
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

