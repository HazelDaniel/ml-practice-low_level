#include <stdio.h>
#include <stdlib.h>

int main (void) {
	char *name = malloc(20 * sizeof(char));
	if (name == NULL) {
		printf("Memory allocation failed \n");
		return 1;
	}
	printf("Enter your name: ");
	scanf("%19s", name);

	printf("your name was %s\n", name);

	free(name);

	return 0;
}
