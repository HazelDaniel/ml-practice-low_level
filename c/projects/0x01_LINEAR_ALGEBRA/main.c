#include "main.h"
// #include "include/vector.h"
#include "include/matrix.h"
#include <stdio.h>

int main(void) {
  Matrix *A = create_matrix(2, 3, 0);
  Matrix *B = create_matrix(3, 2, 0);

  if (!A || !B) {
    printf("Memory allocation failed!\n");
    return -1;
  }

  mat_set(A, 1, 1, 1.0f); mat_set(A, 1, 2, 2.0f); mat_set(A, 1, 3, 3.0f);
  mat_set(A, 2, 1, 4.0f); mat_set(A, 2, 2, 5.0f); mat_set(A, 2, 3, 6.0f);

  mat_set(B, 1, 1, 7.0f); mat_set(B, 1, 2, 8.0f); mat_set(B, 2, 1, 9.0f);
  mat_set(B, 2, 2, 10.0f); mat_set(B, 3, 1, 11.0f); mat_set(B, 3, 2, 12.0f);

  printf("Matrix A:\n");
  print_matrix(A);

  printf("\nMatrix B:\n");
  print_matrix(B);

  Matrix *A_T = transpose_matrix(A);
  printf("\nTranspose of A (view):\n");
  print_matrix(A_T);

  Matrix *C = mat_mul(A, B);
  if (C) {
    printf("\nMatrix C = A * B:\n");
    print_matrix(C);
    destroy_matrix(C);
  }

  Matrix *D = create_matrix(2, 3, 0);
  mat_set(D, 1, 1, 1.0f); mat_set(D, 1, 2, 1.0f); mat_set(D, 1, 3, 1.0f);
  mat_set(D, 2, 1, 1.0f); mat_set(D, 2, 2, 1.0f); mat_set(D, 2, 3, 1.0f);

  Matrix *E = mat_add(A, D);
  if (E) {
    printf("\nMatrix E = A + D:\n");
    print_matrix(E);
    destroy_matrix(E);
  }

  destroy_matrix(D);

  D = create_matrix(2, 2, 0);
  if (D) {
    mat_set(D, 1, 1, 1.0f);
    mat_set(D, 2, 2, 1.0f);
    mat_set(D, 2, 1, 1.0f);

    _Bool x = is_symmetric(D);

    printf("\n Diagonal Matrix D:\n");
    print_matrix(D);
    printf(" is Matrix D symmetric?:\t %s\n", x == 1 ? "true": "false");
  }

  destroy_matrix(A_T);
  destroy_matrix(A);
  destroy_matrix(B);
  destroy_matrix(D);

  return 0;
}
