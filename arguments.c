#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/**
 * argc is the number of arguments passed to the program
 * argv is an array of strings containing the arguments
 * argv[0] is the name of the program
 * argv[1] is the first argument
 * argv[2] is the second argument
 */
int main(int argc, char *argv[]) {
	printf("Number of args: %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("argument %d: %s\n", i, argv[i]);
    }
	
    printf("\n");
    return 0;
}