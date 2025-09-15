#ifndef ___MATRIX_H___
#define ___MATRIX_H___

#include "text_buffer.h"


typedef struct {
  int rows;
  int columns;
  float *data;
} Matrix;

Matrix *create_matrix(unsigned int r, unsigned int c, int init);
void destroy_matrix(Matrix *m);
void print_matrix(Matrix *m);
Matrix *add_matrices(Matrix *m, Matrix *n);
int mat_validate(Matrix *m, unsigned int r, unsigned int c);
void mat_set(Matrix *m, unsigned int r, unsigned int c, float d);
float mat_get(Matrix *m, int r, int c);
Matrix *create_matrix(unsigned int r, unsigned int c, int init);
TextBuffer *mat_to_text_buffer(Matrix *m);


#endif /* ___MATRIX_H___ */
