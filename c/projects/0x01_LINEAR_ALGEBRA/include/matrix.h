#ifndef ___MATRIX_H___
#define ___MATRIX_H___

#include "globals.h"
#include "stdint.h"
#include "text_buffer.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct Vector Vector;
typedef struct VectorSet VectorSet;

typedef struct Matrix {
  struct Matrix        *owner;
  float                *data;
  int                  rows;
  int                  columns;
  size_t               view_vectors;
  size_t               view_matrices;
  uint8_t              is_transposed;
  uint8_t              is_view;
} Matrix;

/* PRIMITIVES */
Matrix *create_matrix(unsigned int r, unsigned int c, int init);
Matrix *create_matrix_view(Matrix *m);
Matrix *transpose_matrix(Matrix *m);
Matrix *mat_from_vec_set(VectorSet *vs);
Matrix *orthogonalize_matrix(Matrix *m, bool copy, VecOrientation o);
Matrix *add_matrices(Matrix *m, Matrix *n);
Matrix *mat_add(Matrix *m, Matrix *n);
Matrix *mat_mul(Matrix *m, Matrix *n);

Vector *get_matrix_slice(Matrix *m, size_t index, VecOrientation o);
Vector *mat_vec_mul(Matrix *a, Vector *v);

TextBuffer *mat_to_text_buffer(Matrix *m);

/* UTILITIES AND OPS */
void set_matrix_slice(Matrix *m, size_t index, Vector *v);
void unitize_matrix(Matrix *m, VecOrientation o);
void destroy_matrix(Matrix *m);
void print_matrix(Matrix *m);
int mat_validate(Matrix *m, unsigned int r, unsigned int c);
void mat_set(Matrix *m, unsigned int r, unsigned int c, float d);
float mat_get(Matrix *m, int r, int c);
_Bool mat_cmp(Matrix *m, Matrix *n);
_Bool is_symmetric(Matrix *m);


#endif /* ___MATRIX_H___ */
