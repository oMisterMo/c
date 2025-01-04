#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// rand() is a function that generates a random number
int main() {
	for (int i = 0; i < 10; i++) {
		printf("rand is = %d\n", rand());
	}
}

