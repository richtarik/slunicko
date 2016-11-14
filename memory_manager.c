#include "memory_manager.h"

/*
 * Ukazatel na naposled přidaný blok
 */
static memory_block_ptr mm_last_block = NULL;

/*
 * Přidá nový blok s ukazatelem na alokovanou pamet
 */
void* memory_manager_malloc(size_t size)
{
	void* ptr = malloc(size);
	
	if(ptr == NULL)
	{
		error_f(ERROR_INTERN);
	}
	
	if(!memory_manager_new_block(ptr))
	{
		free(ptr);
		error_f(ERROR_INTERN);
	}
	
	return ptr;
}

/*
 * Vytvoří nový blok a uloží do něj ukazatel na alokovanou paměť
 */
bool memory_manager_new_block(void* ptr)
{
	memory_block_ptr new = (memory_block_ptr) malloc(sizeof(struct memory_block));
	
	if(new == NULL)
	{
		return false;
	}
	
	new->ptr = ptr;
	new->prev = mm_last_block;
	mm_last_block = new;
	
	return true;
}

/*
 * Realokuje pamet danou ukazatelem
 */
void* memory_manager_realloc(void* ptr, size_t size)
{
	if(mm_last_block == NULL || ptr == NULL)
	{
		return NULL;
	}
	
	memory_block_ptr tmp = mm_last_block;
	
	while(tmp != NULL)
	{
		if(tmp->ptr == ptr)
		{
			break;
		}
		
		tmp = tmp->prev;
	}
	
	if(tmp == NULL)
	{
		return NULL;
	}
	
	tmp->ptr = realloc(ptr, size);
	
	if(tmp->ptr == NULL)
	{
		error_f(ERROR_INTERN);
	}
	
	return tmp->ptr;
}

/*
 * Uvolni vsechnu alokovanou pamet
 */
void memory_manager_free_all()
{
	memory_block_ptr tmp;
	
	while(mm_last_block != NULL)
	{
		tmp = mm_last_block;
		mm_last_block = mm_last_block->prev;
		memory_manager_free_block(tmp);
	}
}

/*
 * Uvolni konkretni alokovanou pamet
 */
void memory_manager_free_one(void* ptr)
{
	if(ptr == NULL)
	{
		return;
	}
	
	memory_block_ptr tmp = mm_last_block;
	memory_block_ptr tmp2;
	
	if(mm_last_block != NULL)
	{
		if(mm_last_block->ptr == ptr)
		{
			mm_last_block = mm_last_block->prev;
			memory_manager_free_block(tmp);
			return;
		}
		
		while(tmp != NULL)
		{
			if(tmp->ptr == ptr)
			{
				tmp2->prev = tmp->prev;
				memory_manager_free_block(tmp);
				return;
			}
			
			tmp2 = tmp;
			tmp = tmp->prev;
		}
	}
}

/*
 * Uvolni alokovanou pamet v bloku a pak jej odstrani
 */
void memory_manager_free_block(memory_block_ptr block)
{
	if(block != NULL)
	{
		free(block->ptr);
		free(block);
	}
}
