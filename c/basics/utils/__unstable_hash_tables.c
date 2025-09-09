#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "__unstable_hash_tables.h"

hash_table_p_t *f_hash_table_create(unsigned long size) {
  hash_table_p_t *table = (hash_table_p_t *)malloc(sizeof(hash_table_p_t));

	if (!table || !size)
	{
		free(table);
		return (NULL);
	}

	table->size = size;
	table->array = (hash_node_p_t **)calloc(size, sizeof(hash_node_p_t *));

	if (!table->array)
	{
		free(table);
		return (NULL);
	}

	return (table);
}

unsigned long int hash_djb2(const unsigned char *str)
{
	unsigned long int hash;
	int c;

	hash = 5381;
	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return (hash);
}

/**
 * key_index - a function that returns the index
 * of a key in a hash table
 * @key: the key provided
 * @size: the size of the hash table
 * Return: unsigned long int
 **/
unsigned long int key_index(const unsigned char *key, unsigned long size)
{
	unsigned long int hash = hash_djb2(key);

	return (hash % size);
}


/**
 * null_like - a function that tells
 * if a string is either an empty string or NULL
 * @s: the input string
 * Return: int
 **/
int null_like(char *s)
{
	if (!s || !*s)
		return (1);
	return (0);
}

char *uint_to_string(unsigned int num) {
  char temp[20];
  char *buffer = malloc(20 * sizeof(char));
  int i = 0;

  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return buffer;
  }

  while (num > 0) {
    temp[i++] = (num % 10) + '0';
    num /= 10;
  }

  int j = 0;
  while (i > 0) {
    buffer[j++] = temp[--i];
  }
  buffer[j] = '\0';

  return buffer;
}

int f_hash_table_set(hash_table_p_t *ht, const unsigned int key_, const uintptr_t value)
{
	int index;
	char *key = uint_to_string(key_);
	hash_node_p_t *new = (hash_node_p_t *)malloc(sizeof(hash_node_p_t)),
	*curr, *prev = NULL;


	if (!ht || !new || null_like((char *)key))
	{
		free(new), free(key);
		return (0);
	}
	index = key_index((const unsigned char *)key, ht->size);
	new->key = strdup(key), new->next = NULL;
	new->value = value;

	if (ht->array[index])
	{
		curr = ht->array[index];
		while (curr && strcmp(curr->key, key))
			prev = curr, curr = curr->next;
		if (!curr)
		{
			new->next = ht->array[index];
			ht->array[index] = new;
			free(key);
			return (1);
		}
		if (!strcmp(curr->key, key))
		{
			new->next = curr->next;
			if (prev)
				prev->next = new;
			else
				ht->array[index] = new;
			free(curr->key), free(curr);
			free(key);
			return (1); /* already set, exact value */
		}
	}

	ht->array[index] = new;
	free(key);
	return (1);
}


void *f_hash_table_get(const hash_table_p_t *ht, const unsigned int key_)
{
	unsigned long int index = 0;
	hash_node_p_t *curr;
	char *key = uint_to_string(key_);

	if (!ht || null_like((char *)key)) {
	  free(key);
		return (void *)0x00;
	}
	index = key_index((const unsigned char *)key, ht->size);
	if (ht->array[index])
	{
		curr = ht->array[index];
		while (curr && strcmp(curr->key, key))
			curr = curr->next;
		if (!curr) {
      free(key);
			return (void *)0x00;
		}
		if (!strcmp(curr->key, key))
		{
		  free(key);
			return curr ? ((void *)curr->value) : (void *)0x00;
		}
	}

	free(key);
	return (void *)0x00;
}


void f_hash_table_delete(hash_table_p_t *ht)
{
	hash_node_p_t *curr, *next;
	unsigned long int i;

	if (!ht)
		return;

	for (i = 0; i < ht->size; i++)
	{
		if (!ht->array[i])
			continue;
		curr = ht->array[i];

		while (curr)
		{
			next = curr->next;
			free(curr->key), free(curr);
			curr = next;
		}
	}
	free(ht->array);
	free(ht);
}
