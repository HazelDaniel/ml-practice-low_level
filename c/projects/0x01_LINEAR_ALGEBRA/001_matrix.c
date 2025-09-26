#include <stdio.h>
#include "include/globals.h"
#include "include/matrix.h"
#include "include/vector.h"


Vector *mat_vec_mul(Matrix *m, Vector *v) {
  Vector *result = NULL, *col_vec = NULL, *row_vec = NULL;

  if (!m || !v) {
    printf("[memory error/mat_vec_mul]: either matrix or vector isn't allocated!\n");
    return result;
  }

  if (v->orientation == VERTICAL && (m->columns != v->size) || v->orientation == HORIZONTAL && (m->rows != v->size)) {
    printf("[input error/mat_vec_mul]: dimension mismatch between matrix and vector!\n");
    return result;
  }

  switch (v->orientation) {
    case HORIZONTAL: {
      result = init_vector(m->columns);
      if (!result) return result;

      for (int i = 0; i < result->size; i++) {
        col_vec = get_matrix_slice(m, (i + 1), VERTICAL);
        vec_set(result, (i + 1), vec_dot(col_vec, v));
        destroy_vector(col_vec);
      }
      break;
    };
    case VERTICAL: {
      result = init_vector(m->rows);
      if (!result) return result;

      for (int i = 0; i < result->size; i++) {
        row_vec = get_matrix_slice(m, (i + 1), HORIZONTAL);
        vec_set(result, (i + 1), vec_dot(row_vec, v));
        destroy_vector(row_vec);
      }
      break;
    };
    default: {
      printf("[input error]: INVALID ORIENTATION!\n");
      exit(1);
    }
  }

  return result;
}
