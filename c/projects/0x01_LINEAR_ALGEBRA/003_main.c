#include "include/vector.h"
#include <stdio.h>

int main(void) {
  float arr_a[] = {1, 2, 3};
  float arr_b[] = {4, 5, 6};

  Vector *a = init_vector(3);
  Vector *b = init_vector(3);

  vec_set(a, arr_a, 3);
  vec_set(b, arr_b, 3);

  Vector *proj = vec_project(a, b);

  printf("Projection of a onto b:\n");
  print_vec(proj);
  printf("\n");

  destroy_vector(a);
  destroy_vector(b);
  destroy_vector(proj);

  return 0;
}
