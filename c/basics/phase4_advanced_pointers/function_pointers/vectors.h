#ifndef ___VECTORS_H___
#define ___VECTORS_H___

#include "f_pointers.h"
#include "binary_fns.h"


double dot_custom(Vector *u, Vector *v, binary_op f);
Vector *apply_to_vec2(Vector *v, double x, binary_op f);


#endif /* ___VECTORS_H___ */
