#include <stdio.h>
#include <stdlib.h>
#include "include/vector.h"
#include "include/matrix.h"
#include "include/globals.h"


Matrix *create_matrix(unsigned int r, unsigned int c, int init) {
  Matrix *new_mat = malloc(sizeof(Matrix));

  new_mat->rows = r;
  new_mat->columns = c;
  new_mat->data = calloc((r * c), sizeof(float));
  new_mat->is_transposed = 0;
  new_mat->is_view = 0;
  new_mat->view_vectors = 0;
  new_mat->view_matrices = 0;
  new_mat->owner = NULL;

  return new_mat;
}

Matrix *create_matrix_view(Matrix *m) {
  Matrix *n = malloc(sizeof(Matrix));

  return n;
}

Matrix *transpose_matrix(Matrix *m) {
  Matrix *w = create_matrix_view(m);
  Matrix *current = m;

  if (!w) {
    return w;
  }


  if (m->is_view) {

    while (current->owner) { // we assume that is_view is true IFF it has owner
      current = current->owner;
    }
  }

  w->owner = current;
  w->data = current->data;
  w->is_view = 1, w->is_transposed = !m->is_transposed;
  w->rows = m->columns, w->columns = m->rows;
  w->view_vectors = 0, w->view_matrices = 0;
  current->view_matrices++;

  return w;
}

Vector *create_matrix_slice(Matrix *m, size_t index, VecOrientation o) {
  Vector *a = malloc(sizeof(Vector));

  if (!a) {
    return a;
  }

  if (index < 1) {
    printf("[input error]: only 1-based values are allowed! \n");
    return a;
  }

  switch (o) {
    case HORIZONTAL: {
      if (index > m->rows) {
        printf("[input error]: OUT OF BOUNDS!\n");
        free(a);
        return NULL;
      }

      a->size = m->columns;
      break;
    };
    case VERTICAL: {
      if (index > m->columns) {
        printf("[input error]: OUT OF BOUNDS!\n");
        free(a);
        return NULL;
      }

      a->size = m->rows;
      break;
    };
    default: {
      free(a);
      return NULL;
    }
  }

  a->index = index;
  a->data = NULL;
  a->set = m;
  a->is_view = 1;
  a->orientation = o;

  return a;
}

void destroy_matrix(Matrix *m) {
  /**
   * TODO:
   * - instead of throwing error:
   * - flag owner matrix as locked to disallow further referencing
   * - when freeing a view matrix, check if its owner has been locked,
   * - if yes, free the owner as well once its view_vectors & view_matrix count both  hit 0
   */
  if (m->view_vectors) {
    printf("[destruction error]: matrix has view vectors refrencing it. so not destroyed!\n");
    return;
  }

  if (m->view_matrices) {
    printf("[destruction error]: matrix has view matrices refrencing it. so not destroyed!\n");
    return;
  }

  if (m->is_view) {
    m->data = NULL;

    if (m->owner) {
      ((m->owner)->view_matrices)--;
    }

    free(m);
    m = NULL;
    return;
  }

  free(m->data), m->data = NULL;
  free(m);
  m = NULL;
}

void print_matrix(Matrix *m) {

  for (int i = 0; i < (m->rows); i++) {

    for (int j = 0; j < m->columns; j++) {

      if ((j + 1) % (m->columns) == 0) {
        printf("%0.1f\n", mat_get(m, (i + 1), (j + 1)));
      } else {
        printf("%0.1f\t", mat_get(m, (i + 1), (j + 1)));
      }

    }

  }
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

int mat_validate_add(Matrix *a, Matrix *b) {
  if (!a || !a) {
    printf("[mem error]: matri(x)(es) not initialized! \n");
    return -1;
  }

  if ((a->columns != b->columns) || (a->rows != b->rows)) {
    printf("[operation error]: matrices not of the same dimension!\n");
    return -1;
  }

  return 0;
}

int mat_validate_mul(Matrix *a, Matrix *b) {
  if (!a || !b) {
    printf("[mem error]: matri(x)(es) not initialized! \n");
    return -1;
  }

  if (a->columns != b->rows) {
    printf("[operation error]: matrices not of the same dimension!\n");
    return -1;
  }

  return 0;
}

void mat_set(Matrix *m, unsigned int r, unsigned int c, float d) {
  if (mat_validate(m, r, c) == -1) {
    return;
  }

  if (m->is_transposed) {
    (m->data)[((c - 1) * (m->rows)) + r - 1] = d;
    return;
  }

  (m->data)[((r - 1) * (m->columns)) + c - 1] = d;
}

float mat_get(Matrix *m, int r, int c) {
  if (mat_validate(m, r, c) == -1) {
    exit(-1);
  }

  if (m->is_transposed) {
    return *(m->data + (((c - 1) * (m->rows)) + r - 1));
  }

  return *(m->data + (((r - 1) * (m->columns)) + c - 1));
}

_Bool mat_cmp(Matrix *m, Matrix *n) {
  _Bool ans = 1;

  if (mat_validate_add(m, n) == -1) {
    return 0;
  }

  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->columns; j++) {
      ans = ans && (mat_get(m, (i + 1),  (j + 1)) == mat_get(n, (i + 1), (j + 1)));
    }
  }

  return ans;
}

_Bool is_symmetric(Matrix *m) {
  Matrix *w = transpose_matrix(m);
  _Bool ans = mat_cmp(m, w);

  destroy_matrix(w);

  return ans;
}

Matrix *mat_add(Matrix *m, Matrix *n) {
  if (mat_validate_add(m, n) == -1) {
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

Matrix *mat_mul(Matrix *m, Matrix *n) {
  if (mat_validate_mul(m, n) == -1) {
    return NULL;
  }

  Matrix *mul_mat = create_matrix(m->rows, n->columns, 0);

  if (!mul_mat) {
    return mul_mat;
  }

  for (int i = 0; i < m->rows; i++) {

    for (int j = 0; j < n->columns; j++) {
      float sum = 0.0f;

      for (int k = 0; k < n->rows; k++) {
        sum += mat_get(m, i + 1, k + 1) * mat_get(n, k + 1, j + 1);
      }

      mat_set(mul_mat, i + 1, j + 1, sum);
    }
  }

  return mul_mat;
}
