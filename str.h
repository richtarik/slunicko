#ifndef _STR_H
#define _STR_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <limits.h>

#define STR_LEN_INC 10 // velikost pocatecni alokace pro retezec v typu String

/*
 * Struktura pro datový typ String
 */
typedef struct
{
	char* str; // znaky retezce se znakem \0 na konci
	int length; // delka retezce bez znaku \0
	int allocated; // velikost alokovane pameti
} String;

int strInit(String *s);
void strFree(String *s);
int strReadInt();
double strReadDouble();
String *strReadString();
void strPrint(String *s);
int strLength(String *s);
int strCopy(String *s1, String *s2);
String *strSubstr(String *s, int i, int n);
int strCompare(String *s1, String *s2);

#endif
