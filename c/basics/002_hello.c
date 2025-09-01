#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int max (long int x, long int y) {
  return (x * (int)(x > y) + y * (int)(y > x)); // we goin branchleeeeees! lmao, green movement ftw.
}

int main (void) {
  char **tasks = NULL;
  int count = 0;

  while (1) {
    char buffer[100];

    printf("Enter your task (or 'quit' to stop) :>_");
    // scanf(" %99[^\n]", buffer); // space before the '%99' tells scanf to ignore any sequence of whitespace character at the start of input buffer
    fgets(buffer, sizeof buffer, stdin); // better and agnostic of whitespace

    if (strcmp(buffer, "quit\n") == 0) {
      break;
    }

    char *task = malloc((strlen(buffer) + 1) * sizeof(char));

    strcpy(task, buffer);
    task[max(strlen(buffer) - 1, 0)] = '\0';

    tasks = realloc(tasks, (count + 1) * sizeof(char *));

    if (!tasks) return 1;
    tasks[count] = task;

    count++;
  }

  for (int i = 0; i < count; i++) {
    printf("[+]: %s \n", tasks[i]);
    free(tasks[i]);
  }

  free(tasks);
  return 0;
}
