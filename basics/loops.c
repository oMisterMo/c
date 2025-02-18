#include <stdio.h>

void for_loop();
void while_loop();
void do_while_loop();
void copy(char to[], char from[]);

int main() {

    char str[] = "Hello";
    char str2[] = "mo";

    for_loop();
    while_loop();
    do_while_loop();


    // Copy "mo" into "Hello"
    // "m o \0 l o" or "m o \0";
    copy(str, str2);

    printf("\n");
    printf("str: %s\n", str);
    printf("str: %s\n", &str[0]);
    printf("\n");

    printf("FIRST: %p\n", str);
    printf("FIRST: %p\n", &str[0]);
    // for(int i = 0; i < 10; ++i) {
    //     printf("%p - %c\n" , (str + i), str[i]);
    // }

    printf("\n");

    return 0;
}

void for_loop() {
    int i = 0;
    for(; i < 5; i++) {
        printf("Hello\n");
    }
}

void while_loop() {
    int a; 
    int b = 5;
    while( (a = b) ) {
        printf("b: %d\n", b);
        b--;
    }

    printf("\na = %d\n", a);
}

void do_while_loop() {
    int i = 10;
    do {
        // Something
        printf("Something...\n");
        i--;
    } while(i > 0);
}

void copy(char to[], char from[]) {
    int i = 0;
    while((to[i] = from[i]) != '\0') {
        ++i;
    }
}
