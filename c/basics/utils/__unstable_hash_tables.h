#ifndef ___UNSTABLE_HASH_TABLES_H___
#define ___UNSTABLE_HASH_TABLES_H___

#include <stdint.h>

typedef struct hash_node_p
{
  char *key;
  uintptr_t value;
  struct hash_node_p *next;
} hash_node_p_t;

typedef struct hash_table_p
{
	 unsigned long int size;
	 hash_node_p_t **array;
} hash_table_p_t;

/* FORMAL DEFINITIONS */
unsigned long int hash_djb2(const unsigned char *str);
unsigned long int key_index(const unsigned char *key, unsigned long int size);

/* FOR POINTER VALUES */
hash_table_p_t *f_hash_table_create(unsigned long int size);
int f_hash_table_set(hash_table_p_t *ht, const unsigned int key, const uintptr_t value);
void *f_hash_table_get(const hash_table_p_t *ht, const unsigned int key);
void f_hash_table_print(const hash_table_p_t *ht);
void f_hash_table_delete(hash_table_p_t *ht);

/* FOR CONSISTENT RETREIVAL OF FUNCTION POINTERS */

/* UTILS */
int null_like(char *s);
char *_strdcat(char *dest, char *src);
char *_strvcat(char *first_str, ...);
unsigned int _strlen(char *str);
int last_oc_of(char *str, char c);


#endif /*___UNSTABLE_HASH_TABLES_H___ */
