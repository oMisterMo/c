#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/**
 *
 * argv[0] is the name of the program e.g. "arguments.c"
 * argv[1] is the first argument
 * argv[2] is the second argument
 * 
 * @param int argc is the number of arguments passed to the program
 * @param char *argv[] is an array of pointers to characters, each element of the array points to a string (character array)
 * @return int Returns 0 on success
 */
int main(int argc, char *argv[]) {
	printf("Number of args: %d\n\n", argc);

    // Pointer stuff
    // printf("-----------------------------\n");
	// printf("Start address of string: %p\n", argv);
	// printf("First value: %s\n", *argv);
    // printf("-----------------------------\n\n");

    for (int i = 0; i < argc; i++) {
        printf("argument %d: %s\n", i, argv[i]);
    }
	
    printf("\n");
    return 0;
}
