#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define RANDOM_MAX 250

/**
 * rand() generates a random number between 0 and RANDOM_MAX
 */
int main() {

	// When you run this loop, you will get the same random number every time
	for (int i = 0; i < 10; i++) {
		printf("rand is = %d\n", rand());
	}

	// srand() is a function that seeds the random number generator
	srand(time(NULL));

	printf("\n");

	// Random value between 0 and 249
	for (int i = 0; i < 10; i++) {
		printf("rand is = %d\n", rand() % RANDOM_MAX);
	}

}
