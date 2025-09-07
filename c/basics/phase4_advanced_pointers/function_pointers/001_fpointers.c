#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include "f_pointers.h"

float add(float x, float y) {
  return x + y;
}

float sub(float x, float y) {
  return x - y;
}

float mul(float x, float y) {
  return x * y;
}

double cube(double x) {
  return pow(x, 3);
}

double sine(double x) {
  return sin(x);
}

double exponent(double x) {
  return exp(x);
}

double num_derivative(unary_vec_fn f, double x) {
  float h = 0.002;

  return sub(f(x + h), f(x - h))/ (2 * h);
}

void apply_vector(Vector *v, unary_vec_fn f) {
  for (size_t i = 0; i < v->size; i++) {
    (v->data)[i] = f((v->data)[i]);
  }
}

int main (void) {
  Vector *v1 = init_vector(3);
  Vector *v2 = init_vector(3);

  // Populate vectors
  float vals1[] = {1.0f, 2.0f, 3.0f};
  float vals2[] = {4.0f, 5.0f, 6.0f};

  vec_set(v1, vals1, 3);
  vec_set(v2, vals2, 3);

  print_vec(v1);
  apply_vector(v1, exponent);
  print_vec(v1);

  destroy_vector(v1);
  destroy_vector(v2);

  return 0;
}
