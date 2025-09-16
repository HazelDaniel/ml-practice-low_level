#include "main.h"
#include "include/matrix.h"
#include <stdio.h>

void safe_destroy(Matrix **m) {
  if (m && *m) {
    destroy_matrix(*m);
    *m = NULL;
  }
}

void run_test(Matrix *m, bool copy, VecOrientation o, const char *label) {
  printf("\n=== %s ===\n", label);
  printf("Original Matrix:\n");
  print_matrix(m);
  puts("");

  Matrix *Q = orthogonalize_matrix(m, copy, o);

  if (Q) {
    printf("Orthogonalized Matrix (Q):\n");
    print_matrix(Q);
    safe_destroy(&Q);
  } else {
    printf("Orthogonalization failed (possibly rank deficient)\n");
  }
}

// Test case runner
int main (void) {
  // ---------- Test 1: Full rank square matrix ----------
  Matrix *m1 = create_matrix(3, 3, 0);
  mat_set(m1, 1, 1, 1); mat_set(m1, 2, 1, 0); mat_set(m1, 3, 1, 0);
  mat_set(m1, 1, 2, 1); mat_set(m1, 2, 2, 1); mat_set(m1, 3, 2, 0);
  mat_set(m1, 1, 3, 1); mat_set(m1, 2, 3, 1); mat_set(m1, 3, 3, 1);

  run_test(m1, true, VERTICAL, "Test 1: Full Rank 3x3 Matrix");
  safe_destroy(&m1);

  // ---------- Test 2: Rectangular matrix (tall, full column rank) ----------
  Matrix *m2 = create_matrix(4, 2, 0);
  mat_set(m2, 1, 1, 1); mat_set(m2, 2, 1, 2); mat_set(m2, 3, 1, 3); mat_set(m2, 4, 1, 4);
  mat_set(m2, 1, 2, 2); mat_set(m2, 2, 2, 3); mat_set(m2, 3, 2, 4); mat_set(m2, 4, 2, 5);

  run_test(m2, true, VERTICAL, "Test 2: Rectangular 4x2 Matrix");
  safe_destroy(&m2);

  // ---------- Test 3: Rank-deficient matrix ----------
  Matrix *m3 = create_matrix(3, 3, 0);
  // Two columns are linearly dependent
  mat_set(m3, 1, 1, 1); mat_set(m3, 2, 1, 2); mat_set(m3, 3, 1, 3);
  mat_set(m3, 1, 2, 2); mat_set(m3, 2, 2, 4); mat_set(m3, 3, 2, 6); // col2 = 2 * col1
  mat_set(m3, 1, 3, 0); mat_set(m3, 2, 3, 1); mat_set(m3, 3, 3, 0);

  run_test(m3, true, VERTICAL, "Test 3: Rank-Deficient Matrix");
  safe_destroy(&m3);

  // ---------- Test 4: Symmetric matrix ----------
  Matrix *m4 = create_matrix(2, 2, 0);
  mat_set(m4, 1, 1, 1); mat_set(m4, 1, 2, 2);
  mat_set(m4, 2, 1, 2); mat_set(m4, 2, 2, 3);

  run_test(m4, true, VERTICAL, "Test 4: Symmetric Matrix");
  safe_destroy(&m4);

  // ---------- Test 5: Orthonormal matrix already ----------
  Matrix *m5 = create_matrix(2, 2, 0);
  mat_set(m5, 1, 1, 1); mat_set(m5, 2, 1, 0);
  mat_set(m5, 1, 2, 0); mat_set(m5, 2, 2, 1);

  run_test(m5, true, VERTICAL, "Test 5: Already Orthonormal Matrix");
  safe_destroy(&m5);

  printf("\nAll tests finished.\n");
  return 0;
}
