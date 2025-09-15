#include <stdio.h>
#include <stdlib.h>

double binary_multiply(double a, double b) {
  return a * b;
}

double binary_add(double a, double b) {
  return a + b;
}

double binary_divide(double a, double b) {
  if (!b) {
    printf("[operation error/binary_divide]: division by zero\n");
    exit(1);
  }

  return a / b;
}
