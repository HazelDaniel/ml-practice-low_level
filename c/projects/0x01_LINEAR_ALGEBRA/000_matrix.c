#include <stdio.h>
#include <stdlib.h>
#include "include/vector.h"
#include "include/matrix.h"
#include "include/globals.h"
#include "utils/binary_fns.h"


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

Vector *get_matrix_slice(Matrix *m, size_t index, VecOrientation o) {
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
  a->is_view = true;
  a->orientation = o;

  return a;
}

void set_matrix_slice(Matrix *m, size_t index, Vector *v) {
  if (!v) {
    // TODO: handle overwriting ith row and ith column (with bounndary checks) with zeros
    return;
  }

  if (index < 1) {
    printf("[input error/set_matrix_slice]: only 1-based values are allowed! \n");
    return;
  }

  switch (v->orientation) {
    case HORIZONTAL: {
      if (index > m->rows || v->size > m->columns) {
        printf("[input error/set_matrix_slice]: OUT OF BOUNDS!\n");
        printf("size (%d), m.rows (%d), m.columns(%d), index (%d)\n", (int)v->size, (int)m->rows, (int)m->columns, (int)index);
        return;
      }

      break;
    };
    case VERTICAL: {
      if (index > m->columns || v->size > m->rows) {
        printf("[input error/set_matrix_slice]: OUT OF BOUNDS!\n");
        printf("size (%d), m.rows (%d), m.columns(%d), index (%d)\n", (int)v->size, (int)m->rows, (int)m->columns, (int)index);
        return;
      }

      break;
    };
    default: {
      return;
    }
  }

  if (v->orientation == HORIZONTAL) {
    for (int i = 0; i < v->size; i++) {
      mat_set(m, index, (i + 1), vec_get(v, (i + 1)));
    }
  } else {
    for (int i = 0; i < v->size; i++) {
      mat_set(m, (i + 1), index, vec_get(v, (i + 1)));
    }
  }
}

Matrix *mat_from_vec_set(VectorSet *vs) {
  Matrix *m = NULL;
  size_t rows, columns;

  if (!vs || vs->count < 2) {
    if (!vs)
    printf("[memory error/mat_from_vec_set]: vec set is NULL\n");
    else
      printf("[input error/mat_from_vec_set]: vector set has one dimension\n");

    return NULL;
  }

  rows = (vs->orientation == HORIZONTAL) ? vs->count : (vs->elements[0])->size;
  columns = (vs->orientation = HORIZONTAL) ? (vs->elements[0])->size : vs->count;

  m = create_matrix(rows, columns, 0);

  if (!m) return m;

  if (vs->orientation == HORIZONTAL) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        mat_set(m, (i + 1), (j + 1), vec_get(vs->elements[i], (j + 1)));
      }
    }
  } else {
    for (int i = 0; i < columns; i++) {
      for (int j = 0; j < rows; j++) {
        mat_set(m, (i + 1), (j + 1), vec_get(vs->elements[i], (j + 1)));
      }
    }
  }

  return m;
}

void unitize_matrix(Matrix *m, VecOrientation o) {
  if (!m) {
    return;
  }

  size_t length = o == HORIZONTAL ? m->columns : m->rows;


  for (int i = 0; i < length; i++) {
    Vector *entry = get_matrix_slice(m, (i + 1), o);
    Vector *tmp = entry;
    double norm = vec_norm(entry);

    entry = apply_to_vec(tmp, norm, binary_divide);

    if (!entry) {
      destroy_vector(tmp);
      return;
    }

    entry->orientation = o;
    destroy_vector(tmp);
    set_matrix_slice(m, (i + 1), entry);
    destroy_vector(entry);
  }

}

Matrix *orthogonalize_matrix(Matrix *m, bool copy, VecOrientation o) {
  Matrix *ortho_mat = NULL;
  VectorSet *vs = NULL;

  if (!m) {
    return ortho_mat;
  }

  vs = vec_set_from_mat(m, o == HORIZONTAL ? m->rows: m->columns, o, SHALLOW);

  if (!vs) {
    return ortho_mat;
  }

  if (copy) {
    ortho_mat = create_matrix(m->rows, m->columns, 0);

    if (!ortho_mat) {
      destroy_vector_set(vs);
      return ortho_mat;
    }
  }

  Matrix *target = copy ? ortho_mat : m;

  set_matrix_slice(target, 1, (vs->elements)[0]);

  // TODO: logical error in  algo, find and fix
  for (int i = 1; i < vs->count; i++) {
    Vector *sum_vec = init_vector(o == HORIZONTAL ? target->columns : target->rows);
    Vector *curr_vec = (vs->elements)[i];
    Vector *tmp = sum_vec;

    if (!sum_vec || !curr_vec) {
      destroy_vector_set(vs);
      if (copy) destroy_matrix(target);
      return NULL;
    }

    for (int j = 0; j < i; j++) {
      sum_vec = vec_sub(tmp, vec_project((vs->elements)[j + 1], (vs->elements)[j]));

      if (!sum_vec) {
        destroy_vector(tmp);
        destroy_vector_set(vs);
        if (copy) destroy_matrix(target);
        return NULL;
      }
    }

    destroy_vector(tmp);
    tmp = curr_vec;
    curr_vec = vec_sum(tmp, sum_vec);
    destroy_vector(sum_vec);

    set_matrix_slice(target, i, curr_vec);
  }

  // puts("before unitizing matrix: ");
  print_matrix(target);

  unitize_matrix(target, o);
  destroy_vector_set(vs);

  return ortho_mat;
}

void destroy_matrix(Matrix *m) {
  if (!m) {
    return;
  }

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
