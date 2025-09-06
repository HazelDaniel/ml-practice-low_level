#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
  VERTICAL = 0,
  HORIZONTAL
} VecOrientation;

typedef struct {
  size_t size;
  float *data;
  VecOrientation orientation;
} Vector;

Vector *init_vector(size_t size) {
  Vector *new_vec = malloc(sizeof(Vector));

  new_vec->data = calloc(size, sizeof(float));
  new_vec->size = size;
  new_vec->orientation = VERTICAL;

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

  if (v->data) {
    free(v->data);
  }

  free(v);
}

void vec_set(Vector *v, const float *values, size_t count) {
  size_t limit = count < v->size ? count : v->size;

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
    printf("-------\n");
    for (size_t i = 0; i < v->size; i++) {
      if (i == v->size - 1) {
        printf("|%.3f|\n", (v->data)[i]);
        printf("-------\n");
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


int main(void) {
    // Initialize two vectors of size 3
    Vector *v1 = init_vector(3);
    Vector *v2 = init_vector(3);

    // Populate vectors
    float vals1[] = {1.0f, 2.0f, 3.0f};
    float vals2[] = {4.0f, 5.0f, 6.0f};
    vec_set(v1, vals1, 3);
    vec_set(v2, vals2, 3);

    printf("Vector v1:\n");
    print_vec(v1);
    printf("Vector v2:\n");
    print_vec(v2);

    // Sum
    Vector *sum = vec_sum(v1, v2);
    printf("\nSum (v1 + v2):\n");
    print_vec(sum);

    // Sub
    Vector *sub = vec_sub(v1, v2);
    printf("\nSubtraction (v1 - v2):\n");
    print_vec(sub);

    // Dot product
    float dot = vec_dot(v1, v2);
    printf("\nDot product (v1 . v2): %.3f\n", dot);

    // Norms
    printf("\nNorm of v1: %.3f\n", vec_norm(v1));
    printf("Norm of v2: %.3f\n", vec_norm(v2));

    // Change orientation of sum
    sum->orientation = HORIZONTAL;
    printf("\nSum vector (horizontal orientation):\n");
    print_vec(sum);

    // Cleanup
    destroy_vector(v1);
    destroy_vector(v2);
    destroy_vector(sum);
    destroy_vector(sub);

    return 0;
}
