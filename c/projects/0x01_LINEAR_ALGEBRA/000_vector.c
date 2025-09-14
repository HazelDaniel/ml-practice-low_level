#include <math.h>
#include <stdio.h>
#include "include/vector.h"

Vector *init_vector(size_t size) {
  Vector *new_vec = malloc(sizeof(Vector));

  new_vec->data = calloc(size, sizeof(float));
  new_vec->size = size;
  new_vec->orientation = VERTICAL;
  new_vec->is_view = 0;
  new_vec->set = NULL;

  if (!new_vec->data) {
    free(new_vec);
    return NULL;
  }

  return new_vec;
}

void destroy_vector(Vector *v) {

  if (!v) {
    return;
  }

  if (v->is_view) {
    v->set = NULL;
    free(v);
    return;
  }

  if (v->data) {
    free(v->data);
  }

  free(v);
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

  return (v->data)[position - 1];
}

Vector *vec_sum(Vector *a, Vector *b) {
  Vector *sum_vec = NULL;

  if (a->size != b->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
  }

  sum_vec = init_vector(a->size);

  for (size_t i = 0; i < sum_vec->size; i++) {
    (sum_vec->data)[i] = (a->data)[i] + (b->data)[i];
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
    (sub_vec->data)[i] = (a->data)[i] - (b->data)[i];
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
    dot += (a->data)[i] * (b->data)[i];
  }

  return dot;
}

float vec_norm(Vector *v) {
  float norm_square = 0.0f;

  for (size_t i = 0; i < v->size; i++) {
    norm_square += pow(((v->data)[i]), 2);
  }

  return sqrt(norm_square);
}

void print_vec(Vector *v) {
  if (v->orientation == VERTICAL) {
    printf("[vector (%p)]\n", (void *)v);
    printf(".-------\n");
    for (size_t i = 0; i < v->size; i++) {
      if (i == v->size - 1) {
        printf("|%.3f|\n", (v->data)[i]);
        printf(".-------\n");
      } else {
        printf("|%.3f|\n", (v->data)[i]);
      }
    }
  } else {
    printf("[vector (%p)]\t", v);
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
      if (i == v->size - 1) {
        printf("%.3f]\n", (v->data)[i]);
      } else {
        printf("%.3f\t", (v->data)[i]);
      }
    }
  }
}
