#ifndef ___VECTOR_H___
#define ___VECTOR_H___

#include <stdlib.h>
#include <stdbool.h>
#include "globals.h"

typedef struct Matrix Matrix;

typedef struct Vector {
  size_t size;
	size_t index;
	_Bool is_view;
  VecOrientation orientation;
  float *data;
  Matrix *set;
} Vector, *Vector_P;

typedef struct {
  Vector **data;
  size_t count;
} VectorSet;

Vector *init_vector(size_t size);
void destroy_vector(Vector *v);
void vec_set(Vector *v, const float *values, size_t count);
float vec_get(Vector *v, size_t position);
Vector *vec_sum(Vector *a, Vector *b);
Vector *vec_sub(Vector *a, Vector *b);
float vec_dot(Vector *a, Vector *b);
float vec_norm(Vector *v);
void print_vec(Vector *v);
void print_vec_set(VectorSet *set);
void push_vec_to_set(Vector *v, VectorSet *set);
Vector *pop_vec_from_set(size_t index, VectorSet *set);


#endif /* ___VECTOR_H___ */
