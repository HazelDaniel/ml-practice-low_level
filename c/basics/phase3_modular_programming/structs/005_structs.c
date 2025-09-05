#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **lines;
  int count;
  int capacity;
} TextBuffer;

TextBuffer *create_text_buffer() {
  TextBuffer *new_buffer = malloc(sizeof(TextBuffer));

  new_buffer->lines = malloc(10 * sizeof(char *));
  new_buffer->count = 0;
  new_buffer->capacity = 10;

  return new_buffer;
}

void free_text_buffer(TextBuffer *b) {
  if (!b) {
    return;
  }

  if (!b->lines) {
    if (b) {
      free(b);
      return;
    }
    return;
  }

  for (int i = 0; i < b->count; i++) {
    if ((b->lines)[i]) {
      free((b->lines)[i]);
    }
  }

  free(b->lines);
  free(b);
}

void add_line(TextBuffer *b, char *s) {
  int new_capacity = b->capacity;
  char *new_line = malloc(sizeof(char) * strlen(s) + 1);
  strcpy(new_line, s);

  if (b->count >= b->capacity) {
    new_capacity = b->capacity * 2;
    b->lines = realloc(b->lines, sizeof(char *) * (new_capacity));
    b->capacity = new_capacity;
  }

  (b->lines)[b->count] = new_line;
  (b->count)++;
}

void print_lines(TextBuffer *b) {
  if (!b) {
    return;
  }

  printf("[TextBuffer]-----\n");

  for (int i = 0; i < b->count; i++) {
    printf("%s\n", (b->lines)[i]);
  }

  printf("-----\n");
}


int main(void) {
  TextBuffer *new_buffer = create_text_buffer();

  if (!new_buffer) {
    printf("text buffer creation failed\n");
    exit(1);
  }

  add_line(new_buffer, "line 1");
  add_line(new_buffer, "line 2");
  add_line(new_buffer, "line 3");
  add_line(new_buffer, "line 4");
  print_lines(new_buffer);

  free_text_buffer(new_buffer);

  return 0;
}
