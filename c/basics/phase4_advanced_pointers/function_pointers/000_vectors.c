#include "vectors.h"

Vector *apply_to_vec2(Vector *v, double x, binary_op f) {
  Vector *res_vec = init_vector(v->size);

  for (size_t i = 0; i < v->size; i++) {
    (res_vec->data)[i] = f(x, (v->data)[i]);
  }

  return res_vec;
}
