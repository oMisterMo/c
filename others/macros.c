#include <stdio.h>

void standard_predefined_macros();
void object_like_macros();
void function_like_macros();

/**
 * Macros
 * 
 * A macro is a fragment of code which has been given a name.
 * 
 * https://gcc.gnu.org/onlinedocs/gcc-13.3.0/cpp/Macros.html
 */
int main(void) {



    standard_predefined_macros();
    object_like_macros();
    function_like_macros();
    


    printf("\n");

    return 0;
}

/**
 * Standard Predefined Macros
 * 
 * The standard predefined macros are specified by the relevant language standards,
 * so they are available with all compilers that implement those standards.
 * Older compilers may not provide all of them.
 * Their names all start with double underscores. 
 */
void standard_predefined_macros() {
    printf("\n---Standard Predefined Macros---\n");
    printf("file: %s\n", __FILE__);
    printf("line: %d\n", __LINE__);
    printf("date: %s\n", __DATE__);
    printf("time: %s\n", __TIME__);
    printf("stdc: %d\n", __STDC__);
    printf("stdc version: %ld\n", __STDC_VERSION__);
    // printf("cpp: %d\n", __cplusplus);
    // printf("objc: %d\n", __OBJC__);
    // printf("assembler: %d\n", __ASSEMBLER__);
}


void object_like_macros() {
    printf("\n---Object like Macros---\n");
}

void function_like_macros() {
    printf("\n---Function like Macros---\n");
}
