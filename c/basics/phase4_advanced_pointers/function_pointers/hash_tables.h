#ifndef ___HASH_TABLES_H___
#define ___HASH_TABLES_H___

#include <stdlib.h>


typedef double (*bin_op_caller_on_range)(double (*fn)(double), int a, int b, size_t n);
typedef struct hash_node_s {
  char *key;
  bin_op_caller_on_range value;
  struct hash_node_s *next;
} hash_node_t, *hash_node_p_t;
typedef struct hash_table_s {
  unsigned long size;
  hash_node_p_t *array;
} hash_table_t, *hash_table_p_t;

hash_table_p_t f_hash_table_create(unsigned long int size);
int f_hash_table_set(hash_table_p_t ht, const unsigned int key, bin_op_caller_on_range value);
bin_op_caller_on_range f_hash_table_get(const  hash_table_p_t ht, const unsigned int key);
void f_hash_table_print(const  hash_table_p_t ht);
void f_hash_table_delete( hash_table_p_t ht);

#endif /* ___HASH_TABLES_H___ */
