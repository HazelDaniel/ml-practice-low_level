#include <stdio.h>
#include "f_pointers.h"
#include "unary_fns.h"

static hash_table_p_t *integral_hash;

void apply_to_vec2(Vector *v, float x, binary_op f) {
  for (size_t i = 0; i < v->size; i++) {
    (v->data)[i] = f(x, (v->data)[i]);
  }
}

double dot_custom(Vector *u, Vector *v, binary_op f) {
  double dot = 0.0f;

  if (u->size != v->size) {
    printf("[operation error]: vectors not of the same dimension!\n");
    return dot;
  }

  for (size_t i = 0; i < u->size; i++) {
    dot += f((u->data)[i], (v->data)[i]);
  }

  return dot;
}

double reimann_sum(unary_op fn, int a, int b, size_t n) {
  double sum = 0.0f;
  int interval = b - a;
  printf("n is %u\n", (unsigned int)n);

  if (a == b) {
    return 0;
  }

  float h = (float)interval / n;

  // USING MIDPOINT
  for (float i = 1; i <= n; i++) {
    sum += fn(a + i + (i - 1) * (h / 2));
  }

  sum *= h;

  return sum;
}

double trapezoidal_sum(unary_op fn, int a, int b, size_t n) {
  double sum = 0.0f;
  double half_sum = 0.0f;
  int interval = b - a;
  printf("n is %u\n", (unsigned int)n);

  if (a == b) {
    return 0;
  }

  float h = (float)interval / n;

  for (size_t i = 1; i <= n - 1; i++) {
    half_sum += fn(a + i * h);
  }

  sum = half_sum * 2;
  sum += (fn(a) + fn(b));
  sum *= (h / 2);

  return sum;
}

double simpson_sum(unary_op fn, int a, int b, size_t n) {
  double sum = 0.0f;
  double odd_sum = 0.0f;
  double even_sum = 0.0f;
  int interval = b - a;

  if (a == b) {
    return 0;
  }

  float h = (float)interval / n;

  for (size_t i = 1; i <= n - 1; i++) {
    odd_sum += fn(a + i * h);
  }

  for (size_t i = 2; i <= n - 1; i += 2) {
    even_sum += fn(a + i * h);
  }

  even_sum *= 2;
  odd_sum *= 4;
  sum += even_sum + fn(a) + fn(b) + odd_sum;
  sum *= (h / 3);

  return sum;
}

Integrator *create_integrator(unary_op op) {
  Integrator *new_integrator = malloc(sizeof(Integrator));

  new_integrator->op = op;
  new_integrator->rule = RECTANGLE;
  new_integrator->integrate = (integral_sum_fn)reimann_sum;

  return new_integrator;
}

void apply_integrator(IntegrationRule rule, Integrator *integrator) {
  void *v = f_hash_table_get(integral_hash, rule);
  integral_sum_fn fn = (integral_sum_fn)v;

  integrator->rule = rule;
  integrator->integrate = fn;
}

void destroy_integrator(Integrator *i) {
  free(i);
}

int main(void) {
  integral_hash = f_hash_table_create(10);
  double res = 0.0f;

  f_hash_table_set(integral_hash, SIMPSON, (void *)simpson_sum);
  f_hash_table_set(integral_hash, RECTANGLE, (void *)reimann_sum);
  f_hash_table_set(integral_hash, TRAPEZOIDAL, (void *)trapezoidal_sum);

  Integrator *i = create_integrator(square);

  // apply_integrator(TRAPEZOIDAL, i);
  if (i->integrate) {
    res = i->integrate(i->op, 0, 1, 10);
    printf("result of trapezoidal integration is %.3f\n", res);
  } else {
    printf("integration not applied successfully!\n");
  }

  destroy_integrator(i);
  f_hash_table_delete(integral_hash);


  return 0;
}
