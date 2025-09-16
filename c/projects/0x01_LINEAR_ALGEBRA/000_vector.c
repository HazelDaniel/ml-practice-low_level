#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/globals.h"
#include "include/vector.h"
#include "include/matrix.h"
#include "utils/binary_fns.h"


Vector *apply_to_vec(Vector *v, double x, binary_vec_op f) {
  Vector *res_vec = init_vector(v->size);

  for (size_t i = 0; i < v->size; i++) {
    (res_vec->data)[i] = f(vec_get(v, (i + 1)), x);
  }

  return res_vec;
}

Vector *init_vector(size_t size) {
  Vector *new_vec = malloc(sizeof(Vector));

  new_vec->data = calloc(size, sizeof(float));
  new_vec->size = size;
  new_vec->orientation = VERTICAL;
  new_vec->is_view = false;
  new_vec->set = NULL;

  if (!new_vec->data) {
    free(new_vec);
    return NULL;
  }

  return new_vec;
}

Vector *freeze_vec_view(Vector *vec_view) {
  Vector *frozen = NULL;

  if (!vec_view->is_view) {
    printf("[freeze error/freeze_vec_view]: input vector not a view vector!\n");
    return frozen;
  }

  if (!vec_view->set) {
    printf("[memory error/freeze_vec_view]: input vector doesn't have an associated matrix!\n");
    return frozen;
  }

  frozen = malloc(sizeof(Vector));
  frozen->size = vec_view->size;
  frozen->index = 0;
  frozen->orientation = vec_view->orientation;
  frozen->set = NULL; frozen->is_view = false;

  for (int i = 0; i < vec_view->size; i++) {
    frozen->data[i] = vec_get(vec_view, (i + 1));
  }

  return frozen;
}

VectorSet *vec_set_from_mat(Matrix *m, size_t size, VecOrientation o,
VecCopyDepth depth) {
  VectorSet *vs = NULL;

  switch (o) {
    case HORIZONTAL: {
      if (size > m->rows) {
        printf("[input error/vec_set_from_mat]: OUT OF BOUNDS!\n");
        return vs;
      }
      break;
    };
    case VERTICAL: {
      if (size > m->columns) {
        printf("[input error/vec_set_from_mat]: OUT OF BOUNDS!\n");
        return vs;
      }
      break;
    };
    default: {
      printf("[input error/vec_set_from_mat]: INVALID ORIENTATION!\n");
      exit(1);
    }
  }

  vs = malloc(sizeof(VectorSet));
  if (!vs)
    return vs;
  vs->count = 0;
  vs->depth = depth;
  vs->orientation = o;
  vs->elements = malloc(size * sizeof(Vector *));

  if (!vs->elements)
    return NULL;

  if (depth == SHALLOW) {
    for (int i = 0; i < size; i++) {
      vs->elements[i] = get_matrix_slice(m, (i + 1), o);
      vs->count++;
    }
  } else if (depth == DEEP) {
    for (int i = 0; i < size; i++) {
      vs->elements[i] = freeze_vec_view(get_matrix_slice(m, (i + 1), o));
      vs->count++;
    }
  }

  return vs;
}

void destroy_vector(Vector *v) {
  if (!v) {
    return;
  }

  if (v->is_view) {
    if (v->set) {
      (v->set)->view_vectors--;
    }
    v->set = NULL;
    free(v);
    return;
  }

  if (v->data) {
    free(v->data);
    v->set = NULL;
  }

  free(v);
}

void destroy_vector_set(VectorSet *vs) {
  if (!vs) return;

  if (vs->depth == SHALLOW) {
    for (int i = 0; i < vs->count; i++) {
      if (vs->elements[i] && vs->elements[i]->is_view) {
        (vs->elements[i]->set)->view_vectors--;
      }
      free(vs->elements[i]);
    }
  } else {
    for (int i = 0; i < vs->count; i++) {
      if (vs->elements[i] && vs->elements[i]->is_view) {
        (vs->elements[i]->set)->view_vectors--;
      }
      destroy_vector((vs->elements)[i]);
    }
  }
  free(vs->elements);
  vs->elements = NULL;
  free(vs);
}

void vec_set(Vector *v, const float *values, size_t count) {
  size_t limit = count < v->size ? count : v->size;

  if (v->is_view) {
    printf("[WRITE ERROR]: a view vector is immutable, freeze to get a owned copy!\n");
    return;
  }

  if (count < v->size) {
    printf("[size warning]: not enough items to populate vector!\n");
  } else if (count > v->size) {
    printf("[size warning]: more than enough items to populate vector!\n");
  }

  for (size_t i = 0; i < limit; i++) {
    (v->data)[i] = values[i];
  }
}

float vec_get(Vector *v, size_t position) {
  if (position == 0) {
    printf("[input error]: only 1-based values are allowed! \n");
    exit(1);
  }

  if (position > v->size) {
    printf("[input error]: OUT OF VECTOR BOUNDS!\n");
    exit(1);
  }

  if (v->is_view) {
    if (v->set) {
      switch (v->orientation) {
        case HORIZONTAL: {
          return mat_get((v->set), v->index, position);
        };
        case VERTICAL: {
          return mat_get((v->set), position, v->index);
        };
        default: {
          printf("[input error]: INVALID ORIENTATION!\n");
          exit(1);
        }
      }
    } else {
      printf("[vec_get/warning]: this vector doesn't have an owner so, invariant violated.\n");
      return 0.0f;
    }
  }

  return (v->data)[position - 1];
}

Vector *vec_sum(Vector *a, Vector *b) {
  Vector *sum_vec = NULL;

  if (a->size != b->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
  }

  sum_vec = init_vector(a->size);

  for (size_t i = 0; i < sum_vec->size; i++) {
    (sum_vec->data)[i] = vec_get(a, (i + 1)) + vec_get(b, (i + 1));
  }

  return sum_vec;
}

Vector *vec_sub(Vector *a, Vector *b) {
  Vector *sub_vec = NULL;

  if (a->size != b->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
  }

  sub_vec = init_vector(a->size);

  for (size_t i = 0; i < sub_vec->size; i++) {
    (sub_vec->data)[i] = vec_get(a, (i + 1)) - vec_get(b, (i + 1));
  }

  return sub_vec;
}

float vec_dot(Vector *a, Vector *b) {
  float dot = 0.0f;

  if (a->size != b->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
    return dot;
  }

  for (size_t i = 0; i < a->size; i++) {
    dot += vec_get(a, (i + 1)) * vec_get(b, (i + 1));
  }

  return dot;
}

float vec_norm(Vector *v) {
  float norm_square = 0.0f;

  for (size_t i = 0; i < v->size; i++) {
    norm_square += pow((vec_get(v, (i + 1))), 2);
  }

  return sqrt(norm_square);
}

void print_vec(Vector *v) {
  if (v->orientation == VERTICAL) {
    printf("[vector (%p)]\n", (void *)v);
    printf(".-------\n");
    for (size_t i = 0; i < v->size; i++) {
      if (i == v->size - 1) {
        printf("|%.3f|\n", vec_get(v, (i + 1)));
        printf(".-------\n");
      } else {
        printf("|%.3f|\n", vec_get(v, (i + 1)));
      }
    }
  } else {
    printf("[vector (%p)]\t", v);
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
      if (i == v->size - 1) {
        printf("%.3f]\n", vec_get(v, (i + 1)));
      } else {
        printf("%.3f\t", vec_get(v, (i + 1)));
      }
    }
  }
}

Vector *vec_project(Vector *u, Vector *v) {
  Vector *projection = NULL;
  double k = 0.0f;

  if (u->size != v->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
    return projection;
  }

  k = vec_dot(u, v) / vec_dot(v, v);

  projection = apply_to_vec(v, k, binary_multiply);

  return projection;
}
