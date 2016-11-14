#ifndef _IAL_H
#define _IAL_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "str.h"
#include "struct.h"
#include "error.h"
#include "memory_manager.h"

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
	bool value_bool;
	int value_int;
	double value_double;
	String *value_String;
};

typedef struct ht_params {
	char *name;
	token_type type; 
	struct ht_params *next;
} *ht_params_ptr;

typedef struct ht_item_func {
	token_type type; // "struct.h", return type
	int declared;
	int defined;
	ht_params_ptr param; // parametry funkce
	struct hash_table *ht; // tabulka symbolu pro proměnné dané funkce
} *ht_item_func_ptr;

typedef struct ht_item_var {
	token_type type; // "struct.h"
	int defined;
	String *label; // platnost proměnné
	union ht_value value; // data proměnné
} *ht_item_var_ptr;

typedef struct ht_table_item {
	String *key; // název
	ht_item_var_ptr var;
	ht_item_func_ptr func;
	struct ht_table_item *next;
} *ht_table_item_ptr;

typedef struct hash_table {
    unsigned int size;
    ht_table_item_ptr *table_item;
} *hash_table_ptr;

unsigned int hash_function(const char *str, unsigned htab_size);
hash_table_ptr ht_init(unsigned size);
void ht_free(hash_table_ptr ht);
void ht_item_func_clear(ht_item_func_ptr item);
void ht_item_var_clear(ht_item_var_ptr item);
ht_item_var_ptr ht_create_item_var(token_type type, int defined, String *label, union ht_value value);
ht_item_func_ptr ht_create_item_func(token_type type, int declared, int defined, ht_params_ptr param, struct hash_table *ht);
bool ht_insert_item_var(ht_item_var_ptr item, String *key, hash_table_ptr ht);
bool ht_insert_item_func(ht_item_func_ptr item, String *key, hash_table_ptr ht);
ht_table_item_ptr ht_search(String *key, bool func, hash_table_ptr ht);

#endif
