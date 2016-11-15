#include "ial.h"
#include "struct.h"
/*
 * Algoritmus pro vyhledavani - Knuth-Morris-Prattuv algoritmus
 */

/*
 * Hlavni funkce algoritmu
 */
int find(String *s, String *search)
{
	if(s == NULL || search == NULL)
	{
		return -1;
	}

	if(strLength(search) == 0)
	{
		return 0;
	}

	int* fail;

	fail = (int*) memory_manager_malloc(sizeof(int) * search->length);

	kmpGraf(search, fail);

	return kmp(s, search, fail);
}

/*
 * Vytvoreni vektoru fail
 */
void kmpGraf(String *s, int *fail)
{
	int j;

	fail[0] = -1;

	for(int i = 1; i < s->length; i++)
	{
		j = fail[i - 1];

		while(j > 0 && s->str[j] != s->str[i - 1])
		{
			j = fail[j];
		}

		fail[i] = j + 1;
	}
}

/*
 * Funkce s algoritmem pro vyhledavani
 */
int kmp(String *s, String *search, int *fail)
{
	int i, j;

	i = 0;
	j = 0;

	while(i < s->length && j < search->length)
	{
		if(j == -1 || s->str[i] == search->str[j])
		{
			i++;
			j++;
		}
		else
		{
			j = fail[j];
		}
	}

	memory_manager_free_one(fail);
	fail = NULL;

	if(j == search->length)
	{
		return i - search->length;
	}
	
	return -1;
}

/*
 * Algoritmus pro razeni
 */

String *sort(String *s)
{
	int step, j;
	char tmp;
	String *s_tmp;

	if(s == NULL)
	{
		return NULL;
	}

	s_tmp = (String*) memory_manager_malloc(sizeof(String));

	strInit(s_tmp);

	strCopy(s_tmp, s);

	step = s_tmp->length / 2;

	while(step > 0)
	{
		for(int i = step; i < s_tmp->length; i++)
		{
			j = i - step;
			while(j > -1 && s_tmp->str[j] > s_tmp->str[j + step])
			{
				tmp = s_tmp->str[j];
				s_tmp->str[j] = s_tmp->str[j + step];
				s_tmp->str[j + step] = tmp;
				j = j - step;
			}
		}
		step = step / 2;
	}

	return s_tmp;
}

/*
 * Tabulka symbolu
 */

/*
 * Hash funkce
 */
unsigned int hash_function(const char *str, unsigned htab_size)
{
    unsigned int h = 0;
    const unsigned char *p;

    for(p = (const unsigned char*)str; *p != '\0'; p++)
    {
		h = 65599 * h + *p;
	}

    return h % htab_size;
}

/*
 * Funkce pro inicializaci tabulky symbolu
 */
hash_table_ptr ht_init(unsigned size)
{
    hash_table_ptr ht;

    ht = (hash_table_ptr) memory_manager_malloc(sizeof(struct hash_table));

    ht->size = size;

    ht->table_item = (ht_table_item_ptr*) memory_manager_malloc(size * sizeof(ht_table_item_ptr));

	// inicializace položek
    for(unsigned i = 0; i < size; i++ )
    {
		ht->table_item[i] = NULL;
    }

	return ht;
}

/*
 * Funkce pro vycisteni tabulky symbolu a jeji odstraneni
 */
void ht_free(hash_table_ptr ht)
{
	if(ht != NULL)
	{
		for(unsigned i = 0; i < ht->size; i++)
		{
			while(ht->table_item[i] != NULL)
			{
				ht_table_item_ptr tmp = ht->table_item[i];
				ht->table_item[i] = tmp->next;

				if(tmp->func == NULL) // vyčištění struktury proměnné
				{
					memory_manager_free_one(tmp->var);
				}
				else // vyčištění struktury funkce
				{
					ht_item_func_clear(tmp->func);
				}

				memory_manager_free_one(tmp->key);
				memory_manager_free_one(tmp);
			}
		}

		memory_manager_free_one(ht->table_item);
		ht->table_item = NULL;
		memory_manager_free_one(ht);
		ht = NULL;
    }
}

/*
 * Pomocna funkce pro vycisteni itemu funkce
 */
void ht_item_func_clear(ht_item_func_ptr item)
{
	ht_free(item->ht); // vyčištění tabulky symbolů dané funkce

	ht_params_ptr tmp = item->param;

	while(tmp != NULL)
	{
		item->param = tmp->next;
		memory_manager_free_one(tmp->name);
		memory_manager_free_one(tmp);
		tmp = item->param;
	}

	memory_manager_free_one(item);
}

/*
 * Funkce pro vytvoření itemu proměnné
 */
ht_item_var_ptr ht_create_item_var(token_type type, int defined, int offset, bool inicialized)
{
	ht_item_var_ptr new = (ht_item_var_ptr) memory_manager_malloc(sizeof(struct ht_item_var));

	new->type = type;
	new->defined = defined;
    new->offset = offset;
    new->inicialized = inicialized;
    
	return new;
}

/*
 * Funkce pro vytvoření itemu funkce
 */
ht_item_func_ptr ht_create_item_func(token_type type, int offset, int defined, ht_params_ptr param, struct hash_table *ht)
{
	ht_item_func_ptr new = (ht_item_func_ptr) memory_manager_malloc(sizeof(struct ht_item_func));

	new->type = type;
	new->defined = defined;
	new->param = param;
	new->ht = ht;
    new->offset = offset;
    
	return new;
}

/*
 * Funkce pro vlozeni nove proměnné
 */
bool ht_insert_item_var(ht_item_var_ptr item, char *key, hash_table_ptr ht)
{
	int idx = hash_function(key, ht->size);

	ht_table_item_ptr tmp = NULL;
	tmp = ht_search(key, false, ht); // proměnná - false

	if(tmp == NULL)
	{
		ht_table_item_ptr new = (ht_table_item_ptr) memory_manager_malloc(sizeof(struct ht_table_item));

		new->key = key;
		new->var = item;
		new->func = NULL;
		new->next = ht->table_item[idx];
		ht->table_item[idx] = new;

		return true;
	}

	return false; // již existuje
}

/*
 * Funkce pro vlozeni nové proměnné
 */
bool ht_insert_item_func(ht_item_func_ptr item, char *key, hash_table_ptr ht)
{
	int idx = hash_function(key, ht->size);

	ht_table_item_ptr tmp = NULL;
	tmp = ht_search(key, true, ht); // funkce - true

	if(tmp == NULL)
	{
		ht_table_item_ptr new = (ht_table_item_ptr) memory_manager_malloc(sizeof(struct ht_table_item));

		new->key = key;
		new->var = NULL;
		new->func = item;
		new->next = ht->table_item[idx];
		ht->table_item[idx] = new;

		return true;
	}

	return false; // již existuje
}

/*
 * Funkce pro nalezeni itemu
 */
ht_table_item_ptr ht_search(char *key, bool func, hash_table_ptr ht)
{
	int idx = hash_function(key, ht->size);

    if(ht->table_item[idx] != NULL)
	{
		ht_table_item_ptr tmp = ht->table_item[idx];

		while(tmp != NULL)
		{
			// funkce - func == true, proměnná - func == false
			if(strcmp(tmp->key, key) == 0 &&
			((tmp->var != NULL && !func) || (tmp->func != NULL && func)))
			{
				return tmp;
			}

			tmp = tmp->next;
		}
	}

	return NULL;
}
