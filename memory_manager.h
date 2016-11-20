#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "error.h"

/*
 * Struktura pametoveho bloku
 */
typedef struct memory_block {
	void* ptr; // ukazatel na pamet s daty
	struct memory_block* prev; // ukazatel na predchazejici blok
} *memory_block_ptr;

void* memory_manager_malloc(size_t size);
void* memory_manager_realloc(void* ptr, size_t size);
void memory_manager_free_all();
void memory_manager_free_one(void* ptr);
bool memory_manager_new_block(void* ptr); // pomocná funkce
void memory_manager_free_block(memory_block_ptr block); // pomocná funkce

#endif