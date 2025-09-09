#ifndef ___VECTOR_H___
#define ___VECTOR_H___
#include <stdlib.h>


typedef enum { VERTICAL = 0, HORIZONTAL } VecOrientation;

typedef struct {
  size_t size;
  float *data;
  VecOrientation orientation;
} Vector;

Vector *init_vector(size_t size);
void destroy_vector(Vector *v);
void vec_set(Vector *v, const float *values, size_t count);
float vec_get(Vector *v, size_t position);
Vector *vec_sum(Vector *a, Vector *b);
Vector *vec_sub(Vector *a, Vector *b);
float vec_dot(Vector *a, Vector *b);
float vec_norm(Vector *v);
void print_vec(Vector *v);


#endif /* ___VECTOR_H___ */
