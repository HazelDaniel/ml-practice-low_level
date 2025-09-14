#ifndef ___TEXT_BUFFER_H___
#define ___TEXT_BUFFER_H___


typedef struct {
  char **lines;
  int count;
  int capacity;
} TextBuffer;

void free_text_buffer(TextBuffer *);
void add_line(TextBuffer *b, char *s);
TextBuffer *create_text_buffer(int default_capacity);


#endif /* ___TEXT_BUFFER_H___ */
