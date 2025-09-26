#include <stdio.h>
#include "include/matrix.h"
#include "include/vector.h"

int main() {
    // Matrix: 2x3
    // [1 2 3]
    // [4 5 6]
    Matrix *m = create_matrix(2, 3, 0);
    mat_set(m, 1, 1, 1); mat_set(m, 1, 2, 2); mat_set(m, 1, 3, 3);
    mat_set(m, 2, 1, 4); mat_set(m, 2, 2, 5); mat_set(m, 2, 3, 6);

    // Case 1: Vertical vector (3x1)
    Vector *v1 = init_vector(3);
    v1->orientation = VERTICAL;
    vec_set(v1, 1, 1);
    vec_set(v1, 2, 1);
    vec_set(v1, 3, 1);

    Vector *res1 = mat_vec_mul(m, v1);
    printf("Result (vertical vector):\n");
    print_vec(res1);

    // Case 2: Horizontal vector (1x2)
    // Vector [1 2], multiply on the left: v × m
    Vector *v2 = init_vector(2);
    v2->orientation = HORIZONTAL;
    vec_set(v2, 1, 1);
    vec_set(v2, 2, 2);

    Vector *res2 = mat_vec_mul(m, v2);
    printf("Result (horizontal vector):\n");
    print_vec(res2);

    // cleanup
    destroy_matrix(m);
    destroy_vector(v1);
    destroy_vector(v2);
    destroy_vector(res1);
    destroy_vector(res2);

    return 0;
}
