#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int rows;
  int columns;
  float *data;
} Matrix;


Matrix *create_matrix(unsigned int c, unsigned int r, int init) {
  Matrix *new_mat = malloc(sizeof(Matrix));

  new_mat->rows = r;
  new_mat->columns = c;
  new_mat->data = malloc((r * c) * sizeof(float));

  return new_mat;
}

void print_matrix(Matrix *m) {
  for (int i = 0; i < (m->rows) * (m->columns); i++) {
    if ((i + 1) % (m->columns) == 0) { // TODO: not confident of this chunking strategy, look into it later
      printf("%0.1f\n", (m->data)[i]);
    } else {
      printf("%0.1f\t", (m->data)[i]);
    }
  }
}

Matrix *add_matrices(Matrix *m, Matrix *n) {
  if (!m || !n) {
    printf("[mem error]: matri(x)(es) not initialized! \n");
    return NULL;
  }
  if ((m->columns != n->columns) || (m->rows != n->rows)) {
    printf("[operation error]: matrices not of the same dimension!\n");
    return NULL;
  }

  Matrix *sum_mat = create_matrix(m->rows, m->columns, 0);

  if (!sum_mat) {
    return sum_mat;
  }

  for (int i = 0; i < (m->rows * m->columns); i++) {
    (sum_mat->data)[i] = (m->data)[i] + (n->data)[i];
  }

  return sum_mat;
}

int mat_validate(Matrix *m, unsigned int r, unsigned int c) {
  if (!m) {
    printf("[mem error]: matrix not initialized! \n");
    return -1;
  }

  if (r < 1 || c < 1) {
    printf("[input error]: only 1-based values are allowed! \n");
    return -1;
  }

  if ((r > (m->rows)) ||  (c > (m->columns))) {
    printf("[input error]: OUT OF BOUNDS!\n");
    return -1;
  }

  return 0;
}

void mat_set(Matrix *m, unsigned int r, unsigned int c, float d) {
  if (mat_validate(m, r, c) == -1) {
    return;
  }

  (m->data)[((r - 1) * (m->columns)) + c - 1] = d;
}

float mat_get(Matrix *m, int r, int c) {
  if (mat_validate(m, r, c) == -1) {
    exit(-1);
  }

  return (m->data)[((r - 1) * (m->columns)) + c - 1];
}

int main (void) {
  Matrix *new_mat = create_matrix(3, 3, 0);
  Matrix *new_mat_b = create_matrix(3, 3, 0);
  Matrix *sum_mat = NULL;

  mat_set(new_mat, 1, 1, 0.3);
  mat_set(new_mat, 1, 2, 0.6);
  mat_set(new_mat, 1, 3, 0.9);

  mat_set(new_mat, 2, 1, 0.2);
  mat_set(new_mat, 2, 2, 0.4);
  mat_set(new_mat, 2, 3, 0.8);

  mat_set(new_mat, 3, 1, 0.1);
  mat_set(new_mat, 3, 2, 0.2);
  mat_set(new_mat, 3, 3, 0.3);

  mat_set(new_mat_b, 1, 1, -0.3);
  mat_set(new_mat_b, 1, 2, -0.6);
  mat_set(new_mat_b, 1, 3, -0.9);

  mat_set(new_mat_b, 2, 1, -0.2);
  mat_set(new_mat_b, 2, 2, -0.4);
  mat_set(new_mat_b, 2, 3, -0.8);

  mat_set(new_mat_b, 3, 1, -0.1);
  mat_set(new_mat_b, 3, 2, -0.2);
  mat_set(new_mat_b, 3, 3, -0.3);

  sum_mat = add_matrices(new_mat, new_mat_b);

  if (sum_mat) {
    print_matrix(sum_mat);
    free(sum_mat->data);
    free(sum_mat);
  }

  free(new_mat->data);
  free(new_mat);

  free(new_mat_b->data);
  free(new_mat_b);


  return 0;
}
