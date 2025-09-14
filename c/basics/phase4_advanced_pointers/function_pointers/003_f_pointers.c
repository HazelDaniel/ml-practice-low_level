#include <stdio.h>
#include "vectors.h"
#include "vector.h"
#include "binary_fns.h"

Vector *project(Vector *u, Vector *v) {
  Vector *projection = NULL;
  double k = 0.0f;

  if (u->size != v->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
    return projection;
  }

  k = vec_dot(u, v) / vec_dot(v, v);

  projection = apply_to_vec2(v, k, binary_multiply);

  return projection;
}

_Bool is_orthogonal(Vector *u, Vector *v, float tolerance) {
  return vec_dot(u, v) <= tolerance;
}

int main(void) {
  return 0;
}
