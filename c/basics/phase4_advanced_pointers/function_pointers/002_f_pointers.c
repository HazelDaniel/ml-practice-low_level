#include <stdio.h>
#include "f_pointers.h"
#include "hash_tables.h"
#include "unary_fns.h"

static hash_table_p_t integral_hash;

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

double riemann_sum(unary_op fn, int a, int b, size_t n) {
  double sum = 0.0f;
  int interval = b - a;

  if (a == b) {
    return 0;
  }

  float h = (float)interval / n;

  // USING MIDPOINT
  for (float i = 1; i <= n; i++) {
    sum += fn(a + (i - 0.5f) * (h));
  }

  sum *= h;

  return sum;
}

double trapezoidal_sum(unary_op fn, int a, int b, size_t n) {
  double sum = 0.0f;
  double half_sum = 0.0f;
  int interval = b - a;

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

  if (n % 2) {
    printf("[operation error]: invalid size of n.\
      simpson's rule allows for only even n!\n");

    return sum;
  }

  if (a == b) {
    return 0;
  }

  float h = (float)interval / n;

  for (size_t i = 1; i <= n - 1; i += 2) {
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
  new_integrator->integrate = (bin_op_caller_on_range)riemann_sum;

  return new_integrator;
}

void apply_integrator(IntegrationRule rule, Integrator *integrator) {
  void *v = f_hash_table_get(integral_hash, rule);
  bin_op_caller_on_range fn = (bin_op_caller_on_range)v;

  integrator->rule = rule;
  integrator->integrate = fn;
}

void destroy_integrator(Integrator *i) {
  free(i);
}

int main(void) {
  double res = 0.0f;

  integral_hash = f_hash_table_create(10);
  f_hash_table_set(integral_hash, SIMPSON, (bin_op_caller_on_range)simpson_sum);
  f_hash_table_set(integral_hash, RECTANGLE, (bin_op_caller_on_range)riemann_sum);
  f_hash_table_set(integral_hash, TRAPEZOIDAL, (bin_op_caller_on_range)trapezoidal_sum);

  Integrator *i = create_integrator(square);

  IntegrationRule rules[] = {RECTANGLE, TRAPEZOIDAL, SIMPSON};
  const char *names[] = {"Rectangle", "Trapezoidal", "Simpson"};

  for (int k = 0; k < 3; k++) {
    apply_integrator(rules[k], i);
    if (i->integrate) {
      res = i->integrate(i->op, 0, 1, 100);
      printf("%s Rule: result = %.6f\n", names[k], res);
    }
  }

  destroy_integrator(i);
  f_hash_table_delete(integral_hash);


  return 0;
}
