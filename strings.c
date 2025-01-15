#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initialize();
void copy(char a[], char b[]);
void copy_string();

int main() {

    initialize();
    copy_string();


    printf("\n");
    return 0;
}

void initialize() {
    // String literal (automatically appends '\0')
    char greetings[] = "Hello World!";
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 26 characters

    // Character array (must manually append '\0')
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};

    // Dynamically allocate
    char *string = malloc(4);
    string[0] = 'h';
    string[1] = 'e';
    string[2] = 'y';
    string[3] = '\0';

    printf("\n%s\n\n", string);


    // It is also important that you know that sizeof will always return the memory size (in bytes), and not the actual string length
    printf("alphabet characters: %ld\n", strlen(alphabet));   // 26 characters
    printf("alphabet bytes: %ld\n\n", sizeof(alphabet));   // 27 bytes (26 + '\0')
    printf("%s\n", greetings);
    printf("%s\n", str);
    printf("%c\n", greetings[1]);
    str[0] = 'J';
    printf("%s\n", str);


    free(string);
}

void copy(char a[], char b[]) {
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
