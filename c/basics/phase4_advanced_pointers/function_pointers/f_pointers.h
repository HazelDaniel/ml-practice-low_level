#ifndef ___F_POINTERS___
#define ___F_POINTERS___

#include "../../include/barrel.h"
#include "hash_tables.h"
// #include "barrel.h"


typedef double (*unary_vec_fn)(double);
typedef enum {
  RECTANGLE = 0,
  TRAPEZOIDAL,
  SIMPSON
} IntegrationRule;
typedef double (*unary_op)(double);
typedef double (*binary_op)(double, double);
typedef struct {
  bin_op_caller_on_range integrate;
  IntegrationRule rule;
  unary_op op;
} Integrator;

Integrator *create_integrator(unary_op op);
void apply_integrator(IntegrationRule, Integrator*);
void apply_to_vec(Vector *v, unary_vec_fn f);
double num_derivative(unary_vec_fn f, double x);
double reimann_sum(unary_op, int, int, size_t);
double trapezoidal_sum(unary_op, int, int, size_t);
double simpson_sum(unary_op, int, int, size_t);


#endif /* ___F_POINTERS___ */
