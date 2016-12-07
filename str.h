#ifndef _STR_H
#define _STR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include "error.h"
#include "memory_manager.h"
#include "struct.h"
#define STR_LENGTH 10 // velikost pocatecni alokace pro retezec v typu String

/*
 * Struktura pro datový typ String
 */
 typedef struct
{
	char* str; // znaky retezce se znakem \0 na konci
	int length; // delka retezce (\0 není započítán)
	int allocated; // velikost alokovane pameti
} String;

void strInit(String *s);
void strAddChar(String *s, char c);
void strFree(String *s);
int strReadInt();
double strReadDouble();
String *strReadString();
void strPrintStr(String *s);
int strLength(String *s);
void strCopy(String *s1, String *s2);
String *strSubstr(String *s, int i, int n);
int strCompare(String *s1, String *s2);
void strLoad(String *s, char *c);
void strCat(String *s, char *c);

String * actTrieda; //v akej triede som aktualne

#endif
