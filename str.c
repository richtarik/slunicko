#include "str.h"

/*
 * Inicializace String
 */
int strInit(String *s)
{
	if((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
	{
		return 1;
	}
	
	s->str[0] = '\0';
	s->length = 0;
	s->allocated = STR_LEN_INC;
	
	return 0;
}

/*
 * Uvolneni String z pameti
 */
void strFree(String *s)
{
	free(s->str);
	free(s);
}

/*
 * Nacte int ze stdin
 */
int strReadInt()
{
	long int cislo;
	int c, velikost = 1, delka = 0;
	char* buffer = (char*) malloc(sizeof(char));
	
	if(buffer == NULL)
	{
		return 1;
		// chyba
	}
	
	while(1)
	{
		c = getchar();
		
		if(velikost == delka)
		{
			velikost *= 2;
			if((buffer = (char*) realloc(buffer, sizeof(char) * velikost)) == NULL)
			{
				free(buffer);
				buffer = NULL;
				return 1;
				// chyba
			}
		}
		
		if(c == '\n' || c == EOF)
		{
			buffer[delka] = '\0';
			break;
		}
		
		if(isdigit(c))
		{
			buffer[delka] = c;
		}
		else
		{
			free(buffer);
			buffer = NULL;
			return 1;
			// chyba
		}
		
		delka++;
	}
	
	sscanf(buffer, "%ld", &cislo);
	
	free(buffer);
	buffer = NULL;
	
	if(cislo >= INT_MAX)
	{
		return INT_MAX;
	}
	return (int)cislo;
}

/*
 * Vrati delku retezce (bez '\0')
 */
int strLength(String *s)
{
	return s->length;
}

/*
 * Prekopiruje druhy retezec do prvniho
 */
int strCopy(String *s1, String *s2)
{
   if (s2->length >= s1->allocated)
   {
      if ((s1->str = (char*) realloc(s1->str, s2->length + 1)) == NULL)
      {
         return 1;
      }
      s1->allocated = s2->length + 1;
   }
   
   strcpy(s1->str, s2->str);
   s1->length = s2->length;
   return 0;
}

/*
 * Vrati podretezec retezce od i, n znaku
 */
String *strSubstr(String *s, int i, int n)
{
	if(i > strLength(s))
	{
		return NULL;
	}
	
	if(i + n > strLength(s))
	{
		n = strLength(s) - i;
	}
	
	String *s_tmp;
	
	if((s_tmp = (String*) malloc(sizeof(String))) == NULL || strInit(s_tmp))
	{
		return NULL;
	}
	
	s_tmp->length = n;
	
	int j;
	for(j = 0; j < n; j++)
	{
		s_tmp->str[j] = s->str[i];
		i++;
	}
	s_tmp->str[j] = '\0';
	
	return s_tmp;
}

/*
 * Porovna retezce
 */
int strCompare(String *s1, String *s2)
{
	int num = strcmp(s1->str, s2->str);
	
	if(num > 0)
	{
		return 1;
	}
	else if(num < 0)
	{
		return -1;
	}
	
	return 0;
}

void strPrint(String *s)
{
	printf("%s", s->str);
}
