///* File: token.h             */
///* Autors: Milos Molitoris,  */
///*         Lukas Richtarik,  */
///*         Jiří Čechák	   */
///* Login: xmolit00, xricht25,*/
///* 		xcecha04		   */
///*                           */
///*       IFJ-Projekt         */
///* Datum: 29.09.2016         */
///* Prelozeno: gcc 4.9        */
///* ------- VUT FIT --------- */

#include "lex.h"

//TODO free
//TODO Token init and check reallock ... reallock all token no only data

/*TODO Prebytecné pocátecní císlice  0 v nenulovém císle vedou v rozírení BASE na oktalovou reprezentaci celého císla.
*/
// ignotuje pociatecne 0 aj u celych cisel -> problem pri rozsireni na oktal. TODO repair
// OK Pro celou cást desetinného literálu i exponent platí, e prebytecné pocátecní císlice 0 jsou ignorovány. OK

T_token* token_buffer=NULL; // TODO = NULL ok ?

void get_back_token(T_token *token)
{
    token_buffer = token;
}
void free_token(T_token *token)
{
    memory_manager_free_one(token->value);
    memory_manager_free_one(token);
}

void Init_token(T_token *token)
{
    token->value = memory_manager_malloc(sizeof(char) * base_token_size);
    token->value[0] = '\0';
    token->valMaxsize=base_token_size;
    token->valActsize=0;
}

void get_token(T_token *token,FILE* sourceFile)
{
    lexikal_state state=state_Start;
    int actChar;
    token->valActsize=0;
    bool number_flag = false;
    bool only_dec_octal_number = false;
    char str[50];

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
                    f_Identifier(sourceFile,token);
                    f_findKeyW(token);
                        return;

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
                    comment_in_block(sourceFile);
                    state=state_Start;
                    break;
                }
                else if(actChar == '/')
                {// find new line or EOF
                   comment_in_line(sourceFile);
                   state=state_Start;
                   break;
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
                }

            case state_Number: //TODO pocitadlo cislic - rozdelovat int a double ??
				
                if(isdigit(actChar))
                {
					// osmickove cislo
                    if(token->value[0] == '0')
                    {
						if(actChar > '7')
						{
							error_f(ERROR_LEX);
						}
						
						if(actChar == '0')
						{
							state = state_Octal_prepare;
							break;
						}
						
						state = state_Octal;
						token->value[0] = '\0';
						token->valActsize = 0;
					}

                    f_addChar(actChar,token);
                }
                // binarni cislo
                else if(token->value[0] == '0' && actChar == 'b' && !only_dec_octal_number)
                {
					state = state_Bin_first;
					token->value[0] = '\0';
					token->valActsize = 0;
				}
				// sestnactkove cislo
				else if(token->value[0] == '0' && actChar == 'x' && !only_dec_octal_number)
                {
					state = state_Hex;
					token->value[0] = '\0';
					token->valActsize = 0;
				}
				else if(actChar == '_')
				{
					number_flag = true;
					only_dec_octal_number = true;
					break;
				}
                else if (actChar == 'e' || actChar == 'E')
                {
                    state=state_Double_exp_1;
					f_addChar(actChar,token);
                }
                else if ( isalpha(actChar) || actChar == '$')
                {
					error_f(ERROR_LEX);
                }
                else if (actChar == '.')
                {
                    state=state_Double_dot;
                    f_addChar(actChar,token);
                }
                else
                {// iny -> nepovolenz znak v dalsom nacitany nastane chyba ak bude nejaky operator vsetko je ok
                    
                    if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
                    
                    ungetc(actChar,sourceFile);
                    token->type=token_number_int;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
                number_flag = false;
                break;
                
            case state_Double_dot:
                if( isdigit(actChar) )
                {
                    state=state_Double;
                    f_addChar(actChar,token);
                }
                else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
                else
                {
					error_f(ERROR_LEX);

                }
                number_flag = false;
                break;
                
            case state_Double:
                if( isdigit(actChar) )
                {
                    f_addChar(actChar,token);
                }
                else if (actChar == 'e' || actChar == 'E')
                {
                    state=state_Double_exp_1;
                    f_addChar(actChar,token);
                }
                else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
                else if ( isalpha(actChar) || actChar == '$')
                {
					error_f(ERROR_LEX);
                }
                else
                {  //ak pride operator ok je to cislo ale ak pride iny znaky -> error
					
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
                    ungetc(actChar,sourceFile);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
                number_flag = false;
                break;
                
            case state_Double_exp_1:
				if(actChar == '0')
				{
					state = state_Double_sign_zero;
					f_addChar(actChar,token);
				}
                else if(isdigit(actChar) )
                {
                    state=state_Double_exp;
                    f_addChar(actChar,token);
                }
                else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
                else if (actChar == '+' || actChar == '-')
                {
                    state=state_Double_exp_sign;
                    f_addChar(actChar,token);
                }
                else
                {
					error_f(ERROR_LEX);
                }
                number_flag = false;
				break;

            case state_Double_exp:
                if(isdigit(actChar) )
                {
                    f_addChar(actChar,token);
                }
                else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
                else if ( isalpha(actChar) || actChar == '$')
                {
                    error_f(ERROR_LEX);
                }
                else
                {  // TODO chyba ak pride operator ok je to cislo ale ak pride iny znaky -> error
					
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
                    ungetc(actChar,sourceFile);
                    token->type=token_number_double;
                    token->value[token->valActsize] = '\0';
                    token->valActsize=0;
                    return;
                }
                number_flag = false;
				break;
			
			case state_Double_sign_zero:
				if(actChar == '0')
				{
					break;
				}
				else if(isdigit(actChar))
				{
					state = state_Double_exp;
					token->value[--token->valActsize] = '\0';
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if(isalpha(actChar) || actChar == '$')
				{
					error_f(ERROR_LEX);
                }
                else
                {
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_double;
					
					return;
				}
				number_flag = false;
				break;
				
            case state_Double_exp_sign:
				if(actChar == '0')
				{
					state = state_Double_sign_zero;
                    f_addChar(actChar,token);
				}
                else if(isdigit(actChar))
                {
                    state = state_Double_exp;
                    f_addChar(actChar,token);
                }
                else if(!(actChar == '_' || actChar == '0'))
				{
					error_f(ERROR_LEX);
                }
                break;
                
            case state_String:
                //f_addChar(actChar,token);
                ungetc(actChar,sourceFile);
                check_String(sourceFile,token);
                token->type=token_String;
                return;
                
            case state_Octal_prepare:
				if(isdigit(actChar))
				{
					if(actChar == '8' || actChar == '9')
					{
						error_f(ERROR_LEX);
					}
					
					if(actChar != '0')
					{
						state = state_Octal;
						token->value[0] = '\0';
						token->valActsize = 0;
						f_addChar(actChar,token);
					}
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if (isalpha(actChar) || actChar == '$')
				{
					error_f(ERROR_LEX);
				}
				else
				{
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_int;
					return;
				}
				number_flag = false;
				break;
				
			case state_Octal:
				if(isdigit(actChar))
				{
					if(actChar == '8' || actChar == '9')
					{
						error_f(ERROR_LEX);
					}
					
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if (isalpha(actChar) || actChar == '$')
				{
					error_f(ERROR_LEX);
				}
				else
				{
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_int;
                    
                    // prevod na desitkove cislo
                    
                    long int number = strtol(token->value, NULL, 8);
                    
                    if(number > INT_MAX || 
                    sprintf(str, "%li", number) < 0) // prevod na retezec
					{
						error_f(ERROR_LEX);
					}
					
					token->value[0] = '\0';
					token->valActsize = 0;
					int i = 0;
					while(str[i] != '\0')
					{
						f_addChar(str[i],token);
						i++;
					}
                    
                    return;
				}
				number_flag = false;
				break;
			
			case state_Bin_first:
				if(actChar == '0' || actChar == '1')
				{
					state = state_Bin;
					f_addChar(actChar,token);
				}
				else if(!(actChar == '_'))
				{
					error_f(ERROR_LEX);
				}
				break;
				
			case state_Bin:
				if(actChar == '0' || actChar == '1')
				{
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if (isalpha(actChar) || actChar == '$')
				{
					error_f(ERROR_LEX);
				}
				else
				{
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_int;
                    
                    // prevod na desitkove cislo
                    
                    long int number = strtol(token->value, NULL, 2);
					
                    if(number > INT_MAX || 
                    sprintf(str, "%li", number) < 0) // prevod na retezec
					{
						error_f(ERROR_LEX);
					}
                    
                    token->value[0] = '\0';
					token->valActsize = 0;
					int i = 0;
					while(str[i] != '\0')
					{
						f_addChar(str[i],token);
						i++;
					}
                    
                    return;
				}
				number_flag = false;
				break;
			
			case state_Hex:
				if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					state = state_Hex_continue;
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else
				{
					error_f(ERROR_LEX);
				}
				number_flag = false;
				break;
				
			case state_Hex_continue:
				if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if (actChar == 'p' || actChar == 'P')
                {
                    state = state_Hex_Pp;
					f_addChar(actChar, token);
                }
                else if (actChar == '.')
                {
                    state = state_Hex_dot;
					f_addChar(actChar,token);
                }
				else if (isalpha(actChar) || actChar == '$')
				{
					error_f(ERROR_LEX);
				}
				else
				{
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_int;
                    
                    // prevod na desitkove cislo
                    
                    long int number = strtol(token->value, NULL, 16);
					
                    if(number > INT_MAX || 
                    sprintf(token->value, "%li", number) < 0) // prevod na retezec
					{
						error_f(ERROR_LEX);
					}
					
					return;
				}
				number_flag = false;
				break;
			
			case state_Hex_Pp:
				if(actChar == '0')
				{
					state = state_Hex_Pp_sign_zero;
					f_addChar(actChar,token);
				}
				else if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					state = state_Hex_Pp_exp;
					f_addChar(actChar,token);
				}
				else if (actChar == '+' || actChar == '-')
                {
                    state = state_Hex_Pp_sign;
                    f_addChar(actChar,token);
                }
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else
				{
					error_f(ERROR_LEX);
				}
				number_flag = false;
				break;
			
			case state_Hex_Pp_sign:
				if(actChar == '0')
				{
					state = state_Hex_Pp_sign_zero;
					f_addChar(actChar,token);
				}
				else if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					state = state_Hex_Pp_exp;
					f_addChar(actChar,token);
				}
				else if(!(actChar == '_'))
				{
					error_f(ERROR_LEX);
                }
                break;
            
            case state_Hex_Pp_sign_zero:
				if(actChar == '0')
				{
					break;
				}
				else if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					state = state_Hex_Pp_exp;
					token->valActsize--;
					token->value[token->valActsize] = '\0';
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if(isalpha(actChar) || actChar == '$')
				{
					error_f(ERROR_LEX);
                }
                else
                {
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_double;
                    // převod na desítkové
                    Hex_to_Dec(token);
					return;
				}
				number_flag = false;
				break;
				
			case state_Hex_Pp_exp:
				if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					f_addChar(actChar,token);
				}
				else if(actChar == '_')
				{
					number_flag = true;
					break;
				}
				else if (isalpha(actChar) || actChar == '$' || actChar == '.')
				{
					error_f(ERROR_LEX);
				}
				else
				{
					if(number_flag) // ukonceno _ (podtrzitkem)
					{
						error_f(ERROR_LEX);
					}
					
					ungetc(actChar,sourceFile);
                    token->type = token_number_double;
                    // převod na desítkové
                    Hex_to_Dec(token);
					return;
				}
				number_flag = false;
				break;
			
			case state_Hex_dot:
				if(isdigit(actChar) || 
				(actChar >= 'a' && actChar <= 'f') || 
				(actChar >= 'A' && actChar <= 'F'))
				{
					f_addChar(actChar,token);
				}
				else if (actChar == 'p' || actChar == 'P')
                {
                    state = state_Hex_Pp;
					f_addChar(actChar, token);
                }
				else if(!(actChar == '_'))
				{
					error_f(ERROR_LEX);
				}
				break;
        }
    }
    return;
}

/*
 * Převod čísla v šestnáckové soustavě s exp nebo 
 * s desetinnou částí a exp do desítkové soustavy
 */
void Hex_to_Dec(T_token *token)
{
	char str[50];
	char str2[50];
	bool dot_flag = false;
	long int number;
	long double sum = 0;
	
	String *s = (String*) memory_manager_malloc(sizeof(String));
	strInit(s);
	String *s1 = (String*) memory_manager_malloc(sizeof(String));
	strInit(s1);
	
	// převod celé části
	int i = 0;
	while(1)
	{
		if(token->value[i] == '.')
		{
			dot_flag = true;
			break;
		}
		else if(token->value[i] == 'p' || token->value[i] == 'P')
		{
			break;
		}
		strAddChar(s, token->value[i]);
		i++;
	}
	
	number = strtol(s->str, NULL, 16);
	
	// převod desetinné části
	if(dot_flag)
	{
		i++;
		double m = 16;
		while(token->value[i] != 'p' && token->value[i] != 'P')
		{
			sum += hex_digit_to_dec_digit(token->value[i]) * (1 / m);
			i++;
			m *= 16;
		}
	}
	
	// spojení celé a případné desetinné části
	sum += number;
	
	if(sprintf(str, "%LF", sum) < 0) // prevod na retezec
	{
		error_f(ERROR_LEX);
	}
	
	// převod exponentu
	i++;
	int sign = 0;
		
	if(token->value[i] == '+' || token->value[i] == '-')
	{
		sign = token->value[i];
		i++;
	}
	
	while(token->value[i] != '\0')
	{
		strAddChar(s1, token->value[i]);
		i++;
	}
	
	number = strtol(s1->str, NULL, 16);
	
	if(sprintf(str2, "%li", number) < 0) // prevod na retezec
	{
		error_f(ERROR_LEX);
	}
	
	// spojení částí a přidání do tokenu
	token->value[0] = '\0';
	token->valActsize = 0;
	
	i = 0;
	while(str[i] != '\0')
	{
		f_addChar(str[i], token);
		i++;
	}
	
	f_addChar('e', token);
	
	if(sign != 0)
	{
		f_addChar(sign, token);
	}
	
	i = 0;
	while(str2[i] != '\0')
	{
		f_addChar(str2[i], token);
		i++;
	}
	
	strFree(s);
	strFree(s1);
}

/*
 * Převod hexadecimální číslice do decimální
 */
int hex_digit_to_dec_digit(char c)
{
	switch(c)
	{
		case 'a': 
		case 'A': return 10;
		case 'b': 
		case 'B': return 11;
		case 'c': 
		case 'C': return 12;
		case 'd': 
		case 'D': return 13;
		case 'e': 
		case 'E': return 14;
		case 'f': 
		case 'F': return 15;
		default: return c - '0'; // převod znaku číslice na skutečnou číslici
	}
}

void comment_in_block (FILE* sourceFile)
{
    int c_next;
    while( ( c_next=fgetc(sourceFile) ) ) // TODO check EOF
    {
        if(c_next=='*')
        {
            if( (c_next=fgetc(sourceFile)) == '/')
            {
                return; //koniec komentara
            }
            else
                continue;
        }
        else if(c_next==EOF) //TODO printf chybovy stav
        {
			error_f(ERROR_LEX);
            //fprintf(stderr, "ERROR neukonceny blokovy komentar\n");
        }
        else
            continue;
    }
    //c_next==EOF
    //LEX_ERROR - predcasny koniec
}

void comment_in_line(FILE* sourceFile)
{
    int c_next;
    while( (c_next=fgetc(sourceFile)) != EOF )
    {
        if(c_next=='\n')
        {
            return;
        }
        else if(c_next==EOF) //TODO printf chybovy stav
        {
			error_f(ERROR_LEX);
            //fprintf(stderr, "ERROR neukonceny komentar\n");
        }
        else
            continue;
    }
}

void check_String(FILE* sourceFile, T_token *actToken)
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
            f_addChar(actChar,actToken);
        }
    }

    actToken->value[actToken->valActsize] = '\0';
    actToken->valActsize=0;
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
            if(nextChar >= '0' && nextChar <= '3')
            {
				substring[0] = nextChar;
				nextChar = fgetc(sourceFile);
				
				if(substring[0] == '3' && nextChar > '7')
				{
					break;
				}
				
				if(isdigit(nextChar))
				{
					substring[1] = nextChar;
					nextChar = fgetc(sourceFile);
					
					if(!isdigit(nextChar) || 
					(substring[0] == '3' && substring[1] == '7' && nextChar > '7') ||
					(substring[0] == '0' && substring[1] == '0' && nextChar == '0'))
					{
						break;
					}
					
					convert_result = (substring[0] - 48) * 8 * 8 + (substring[1] - 48) * 8 + (nextChar - 48);
					
					if(convert_result > 0 && convert_result < 378)
					{
						f_addChar(convert_result, actToken);
						return;
					}
				}
            }
            break;
     }
     
     error_f(ERROR_LEX);

}

void f_addChar (char symbol, T_token* token)
{
    if(token->valActsize + 1 >= token->valMaxsize) // TODO ked pridavam posledny znak /0 zmesti sa ??
    {
        token->valMaxsize += ALLOC_BLOCK;
        token->value = memory_manager_realloc(token->value, token->valMaxsize); //zvacsenie bloku pre string v tokene
    }
    token->value[token->valActsize]= symbol;
    token->valActsize+=1;
    token->value[token->valActsize]= '\0';
}

void f_Identifier(FILE* sourceFile, T_token *actToken)
{
    int actChar;
    while( ( actChar= fgetc(sourceFile) ) )
    {
        if( isalpha(actChar) || isdigit(actChar) || actChar=='$' || actChar=='_')
        {
            f_addChar(actChar,actToken);
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
}
