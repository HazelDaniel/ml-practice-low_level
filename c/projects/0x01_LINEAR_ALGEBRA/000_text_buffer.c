#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/matrix.h"
#include "include/text_buffer.h"

//UTILS
/**
 * Overwrite contents of dest with src starting at offset.
 *
 * dest:    dynamically allocated destination string
 * src:     source string to copy
 * offset:  position in dest where overwrite begins
 *
 * Returns n(items copied) on success, -1 on invalid input.
 */
int overwrite_string(char *dest, const char *src, size_t offset, int destlen, int srclen) {
    if (!dest || !src) return -1;

    size_t dest_len = destlen;
    size_t src_len  = srclen;

    if (offset >= dest_len) {
        // offset is out of bounds, nothing to do
        return -1;
    }

    // Calculate how many characters we can actually copy
    size_t max_copy = dest_len - offset;

    // Copy up to max_copy or until end of src
    size_t i;
    for (i = 0; i < max_copy && src[i] != 0; i++) {
        dest[offset + i] = src[i];
    }
    // printf("printed %u elements \n", (unsigned int)i);

    // If src was shorter than available space,
    // just stop at the null terminator and leave the rest of dest as-is.
    return i;
}

TextBuffer *create_text_buffer(int default_capacity) {
  TextBuffer *new_buffer = malloc(sizeof(TextBuffer));

  new_buffer->lines = malloc(default_capacity * sizeof(char *));
  new_buffer->count = 0;
  new_buffer->capacity = default_capacity;

  return new_buffer;
}

TextBuffer *mat_to_text_buffer(Matrix *m) {
  TextBuffer *new_text_buffer = NULL;
  _Bool warn = 0;

  if (!m || !m->data) {
    printf("[mem error]: matrix not initialized! \n");
    return new_text_buffer;
  }

  new_text_buffer = create_text_buffer(m->rows);

  int new_line_cap = ((10 * m->columns) + (m->columns - 1));

  for (int i = 0; i < m->rows; i++) {
    char *new_line = calloc((new_line_cap), sizeof(char)); // magic number 10 for now. we'll scan for max digit later

    if (!new_line) {
      if (i == 0) {
        free_text_buffer(new_text_buffer);
        return NULL;
      } else {
        warn = 1;
      }
    }

    for (int j = 0; j < m->columns; j++) {
      char *new_tok = calloc(10, sizeof(char));

      if (j != 0) {
        overwrite_string(new_line, "\t", ((10 - 1) * j), new_line_cap, 1);
      }

      snprintf(new_tok, 10, "%.7f", mat_get(m, (i + 1), (j + 1)));
      overwrite_string(new_line, new_tok, (j == 0 ? ((10 - 1) * j) : ((10  - 1) * j + 1)), new_line_cap, (10 - 1));

      if (new_tok) {
        free(new_tok);
      }

    }

    add_line(new_text_buffer, new_line);
    free(new_line);
  }

  if (warn) {
    printf("[mem warn]: partial text buffer allocated\n");
  }

  return new_text_buffer;
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

