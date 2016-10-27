#ifndef _IAL_H
#define _IAL_H

#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#include "str.h"
#include "struct.h"

/*
 * Funkce pro vyhledavani podretezce v retezci
 */
int find(String *s, String *search);
void kmpGraf(String *s, int *fail);
int kmp(String *s, String *search, int *fail);

/*
 * Funkce pro razeni retezce
 */
String *sort(String *s);

/*
 * Tabulka symbolu
 */
union ht_value {
	int value_int;
	double value_double;
	String *value_String;
};
 
typedef struct ht_item {
	char *key; // nazev
	token_type type; // "struct.h", u funkce return type
	int declared;
	int defined;
	int function; // funkce, kde byla deklarovana promena nebo oznaceni
	union ht_value value; // data promenne
	struct ht_item *param; // ukazatel na parametr funkce
} *ht_item_ptr;

typedef struct ht_table_item {
	ht_item_ptr item; // obsah polozky v tabulce
	struct ht_table_item *next;
} *ht_table_item_ptr;

typedef struct hash_table {
    unsigned int size;
    ht_table_item_ptr *table_item;
} *hash_table_ptr;

unsigned int hash_function(const char *str, unsigned htab_size);
hash_table_ptr ht_init(unsigned size);
void ht_free(hash_table_ptr ht);
void ht_item_clear(ht_item_ptr item);
ht_table_item_ptr ht_insert(ht_item_ptr item, hash_table_ptr ht);
ht_table_item_ptr ht_search(char *key, hash_table_ptr ht);

#endif
