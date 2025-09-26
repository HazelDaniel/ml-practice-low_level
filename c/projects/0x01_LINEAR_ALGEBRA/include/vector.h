#ifndef ___VECTOR_H___
#define ___VECTOR_H___

#include <stdlib.h>
#include <stdbool.h>
#include "globals.h"


typedef struct Matrix Matrix;

typedef double (*unary_vec_op)(double);
typedef double (*binary_vec_op)(double, double);

typedef struct Vector {
  size_t                size;
	size_t                index;
	_Bool                 is_view;
  VecOrientation        orientation;
  float                 *data;
  Matrix                *set;
} Vector, *Vector_P;

typedef struct VectorSet {
  size_t                count;
  VecOrientation        orientation;
  VecCopyDepth          depth;
  Vector                **elements;
} VectorSet;

Vector *init_vector(size_t size);
VectorSet *vec_set_from_mat(Matrix *m, size_t size, VecOrientation o, VecCopyDepth depth);
void destroy_vector(Vector *v);
void destroy_vector_set(VectorSet *vs);
Vector *freeze_vec_view(Vector *vec_view);
void vec_populate(Vector *v, const float *values, size_t count);
void vec_set(Vector *v, size_t index, float value);
float vec_get(Vector *v, size_t position);
Vector *vec_sum(Vector *a, Vector *b);
Vector *vec_sub(Vector *a, Vector *b);
Vector *vec_project(Vector *a, Vector *b);
float vec_dot(Vector *a, Vector *b);
float vec_norm(Vector *v);
void print_vec(Vector *v);
void print_vec_set(VectorSet *set);
void push_vec_to_set(Vector *v, VectorSet *set);
Vector *pop_vec_from_set(size_t index, VectorSet *set);
Vector *apply_to_vec(Vector *v, double x, binary_vec_op f);


#endif /* ___VECTOR_H___ */
