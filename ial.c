#include "ial.h"

/*
 * Algoritmus pro vyhledavani - Knuth-Morris-Prattuv algoritmus
 */

/*
 * Hlavni funkce algoritmu
 */ 
int find(String *s, String *search)
{
	int* fail;
	
	if((fail = (int*) malloc(sizeof(int) * search->length)) == NULL)
	{
		return -1; // chyba
	}
	
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
	
	free(fail);
	fail = NULL;
	
	if(j == search->length)
	{
		return i - search->length;
	}
	return -1;
}

/*
 * Algoritmus pro trideni
 */

String *sort(String *s)
{
	int step, j;
	char tmp;
	String *s_tmp;
	
	if((s_tmp = (String*) malloc(sizeof(String))) == NULL || strInit(s_tmp) || strCopy(s_tmp, s))
	{
		return NULL;
	}
	
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
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
      h = 65599*h + *p;
    return h % htab_size;
}

/*
 * Funkce pro inicializaci tabulky symbolu
 */ 
hash_table_ptr ht_init(unsigned size)
{
    hash_table_ptr ht;
    
    if((ht = (hash_table_ptr) malloc(sizeof(struct hash_table))) == NULL)
    {
		return NULL;
	}
    
    ht->size = size;
    
    if((ht->table_item = (ht_table_item_ptr*) malloc(size * sizeof(ht_table_item_ptr))) == NULL)
    {
		free(ht);
		return NULL;
	}

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
				ht_item_clear(tmp->item);
				free(tmp->item);
				free(tmp);
			}
		}
		
		free(ht->table_item);
		ht->table_item = NULL;
		free(ht);
		ht = NULL;
    }
}

/*
 * Pomocna funkce pro vycisteni itemu
 */ 
void ht_item_clear(ht_item_ptr item)
{
	ht_item_ptr tmp = item->param;
	
	while(tmp != NULL)
	{
		free(tmp->key);
		item->param = tmp->param;
		free(tmp);
		tmp = item->param;
	}
	
	free(item->key);
}

/*
 * Funkce pro vlozeni noveho itemu
 */ 
bool ht_insert(ht_item_ptr item, hash_table_ptr ht)
{
	int idx = hash_function(item->key, ht->size);
	
	ht_table_item_ptr tmp = ht_search(item->key, ht);
		
	if(tmp == NULL)
	{
		ht_table_item_ptr new = (ht_table_item_ptr) malloc(sizeof(struct ht_table_item));
		
		if(new != NULL)
		{
			new->item = item;
			new->next = ht->table_item[idx];
			ht->table_item[idx] = new;
		}
	}
	else
	{
		return true;
	}
	
	return false;
}

/*
 * Funkce pro nalezeni itemu
 */ 
ht_table_item_ptr ht_search(char *key, hash_table_ptr ht)
{
	int idx = hash_function(key, ht->size);

    if(ht->table_item[idx] != NULL)
	{
		ht_table_item_ptr tmp = ht->table_item[idx];
			
		while(tmp != NULL)
		{
			if(tmp->item->key == key) // dopsat rozliseni funkce/promena!!!!!!
			{
				return tmp;
			}
			
			tmp = tmp->next;
		}
	}
	
	return NULL;
}
