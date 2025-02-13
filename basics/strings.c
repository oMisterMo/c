#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initialize();
void modify();
void copy(char a[], char b[]);
void copy_string();
void array_pointers();

int main() {

    initialize();
    modify();
    copy_string();
    array_pointers();


    printf("\n");
    return 0;
}

void initialize() {
    printf("\n---Initialize---\n");
    // String literal (automatically appends '\0')
    char greetings[] = "Hello World!";
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 26 characters

    // Character array (must manually append '\0') [can't be modified]
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};

    // Pointer which points to a string [can be modified]
    char *str2 = "Some String";

    // Dynamically allocate
    char *string = malloc(4);
    string[0] = 'h';
    string[1] = 'e';
    string[2] = 'y';
    string[3] = '\0';

    printf("%s\n\n", string);


    // It is also important that you know that sizeof will always return the memory size (in bytes), and not the actual string length
    printf("alphabet characters: %ld\n", strlen(alphabet));   // 26 characters
    printf("alphabet bytes: %ld\n\n", sizeof(alphabet));   // 27 bytes (26 + '\0')
    printf("%s\n", greetings);
    printf("%s\n", str);
    printf("%c\n", greetings[1]);
    str[0] = 'J';
    printf("%s\n", str);
    printf("%s\n", str2);


    free(string);
    string = NULL;
}

void modify() {
    printf("\n---Array Modify---\n");
    // char str[] = "Hello";
    char *str = "Hello";

    str = "MO";
    printf("%c\n", *(str+1));

    // Error, strings are stored once in immutable locations
    // If you want to edit character use str[]
    // If you want to edit whole word use char *str
    // *(str+1) = 'P';

    for (int i = 0; i < 10; ++i) {
        printf("%p %c\n", str + i, str[i]);
    }

    printf("\n");
}

void copy(char a[], char b[]) {
    printf("\n---Copy---\n");
    int i = 0;
    while( (a[i] = b[i]) != '\0' ) {
        ++i;
    }
}

/**
 * Copies the string from src to dest
 * Note that the size of dest should be large enough to store the result of the two strings combined
 */
void copy_string() {
    printf("\n---String Copy---\n");
    char src[6] = "hi", dest[6] = "whats";

    printf("Initial source string: %s\n", src);
    printf("Initial destination string: %s\n", dest);

    // Copy bytes from src to dest
    strcpy(dest, src);
    // copy(dest, src);

    printf("Final copied string: %s\n", dest);
}

void array_pointers() {
    printf("\n---Array Pointers---\n");
    char str[] = "Hello";

    // The array name is a pointer to the first value of the list
    printf("%p\n", str);
    printf("%p\n", &str[0]);

    // You can modify pointers
    printf("%c\n", *&str[0]);
    printf("%c\n", *str);
    *str = 'Y';

    // You can not modify arrays
    // str = "Impossible";


    printf("%s\n", str);
}
