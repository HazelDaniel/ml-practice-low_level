#ifndef ___F_POINTERS___
#define ___F_POINTERS___

#include "vector.h"
#include "matrix.h"


typedef double (*unary_vec_fn)(double);

void apply_vector(Vector *v, unary_vec_fn f);
double num_derivative(unary_vec_fn f, double x);


#endif /* ___F_POINTERS___ */
