#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int rows;
  int columns;
  float *data;
} Matrix;


Matrix *create_matrix(unsigned int c, unsigned int r, int init) {
  Matrix *new_mat = malloc(sizeof(Matrix));

  new_mat->rows = r;
  new_mat->columns = c;
  new_mat->data = malloc((r * c) * sizeof(float));

  return new_mat;
}

int mat_validate(Matrix *m, unsigned int r, unsigned int c) {
  if (!m) {
    printf("[mem error]: matrix not initialized! \n");
    return -1;
  }

  if (r < 1 || c < 1) {
    printf("[input error]: only 1-based values are allowed! \n");
    return -1;
  }

  if ((r > (m->rows)) ||  (c > (m->columns))) {
    printf("[input error]: OUT OF BOUNDS!\n");
    return -1;
  }

  return 0;
}

void mat_set(Matrix *m, unsigned int r, unsigned int c, float d) {
  if (mat_validate(m, r, c) == -1) {
    return;
  }

  (m->data)[((r - 1) * (m->columns)) + c - 1] = d;
}

float mat_get(Matrix *m, int r, int c) {
  if (mat_validate(m, r, c) == -1) {
    exit(-1);
  }

  return (m->data)[((r - 1) * (m->columns)) + c - 1];
}

int main (void) {
  Matrix *new_mat = create_matrix(3, 3, 0);

  mat_set(new_mat, 1, 1, 0.3);
  mat_set(new_mat, 1, 2, 0.6);
  mat_set(new_mat, 1, 3, 0.9);

  puts("");
  printf("matrix entry at (1, 1) is: %.1f\n", mat_get(new_mat, 1, 1));
  printf("matrix entry at (1, 2) is: %.1f\n", mat_get(new_mat, 1, 2));
  printf("matrix entry at (1, 3) is: %.1f\n", mat_get(new_mat, 1, 3));
  puts("");

  mat_set(new_mat, 2, 1, 0.2);
  mat_set(new_mat, 2, 2, 0.4);
  mat_set(new_mat, 2, 3, 0.8);

  puts("");
  printf("matrix entry at (2, 1) is: %.1f\n", mat_get(new_mat, 2, 1));
  printf("matrix entry at (2, 2) is: %.1f\n", mat_get(new_mat, 2, 2));
  printf("matrix entry at (2, 3) is: %.1f\n", mat_get(new_mat, 2, 3));
  puts("");

  mat_set(new_mat, 3, 1, 0.1);
  mat_set(new_mat, 3, 2, 0.2);
  mat_set(new_mat, 3, 3, 0.3);

  puts("");
  printf("matrix entry at (3, 1) is: %.1f\n", mat_get(new_mat, 3, 1));
  printf("matrix entry at (3, 2) is: %.1f\n", mat_get(new_mat, 3, 2));
  printf("matrix entry at (3, 3) is: %.1f\n", mat_get(new_mat, 3, 3));
  puts("");


  free(new_mat->data);
  free(new_mat);

  return 0;
}
