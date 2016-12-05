#ifndef _IAL_H
#define _IAL_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "struct.h"
#include "error.h"
#include "memory_manager.h"
#include "str.h"
#include "instrlist.h"

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

typedef struct ht_params {
	char *name;
    token_type type;
    int offset; //offset od zaciatku ramca
	struct ht_params *next;
}*ht_params_ptr;

typedef struct ht_item_func {
	token_type type; // "struct.h", return type
	ht_params_ptr param; // parametry funkce
	T_instr_list *instructionList;
	int nParams;
} *ht_item_func_ptr;

typedef struct ht_item_var {
	token_type type; // "struct.h"
	int offset; //offset od zaciatku ramca funkcie
	bool inicialized;
} *ht_item_var_ptr;

typedef struct ht_table_item {
	char *key; // název
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
ht_item_var_ptr ht_create_item_var(token_type type, int offset, bool inicialized);
ht_item_func_ptr ht_create_item_func(token_type type, T_instr_list *instructionList, ht_params_ptr param, int nParams);
bool ht_insert_item_var(ht_item_var_ptr item, char *key, hash_table_ptr ht);
bool ht_insert_item_func(ht_item_func_ptr item, char *key, hash_table_ptr ht);
ht_table_item_ptr ht_search(char *key, bool func, hash_table_ptr ht);

hash_table_ptr global_table; //tabulky
hash_table_ptr local_table;

#endif
