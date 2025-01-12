#include <stdio.h>
#include <string.h>

void initialize();
void copy();

int main() {

    initialize();
    copy();


    printf("\n");
    return 0;
}

void initialize() {
    printf("\n---String Init---\n");

    // string literal (automatically appends '\0')
    char greetings[] = "Hello World!";
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 26 characters

    // character array (must manually append '\0')
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};


    // It is also important that you know that sizeof will always return the memory size (in bytes), and not the actual string length
    printf("alphabet characters: %ld\n", strlen(alphabet));   // 26 characters
    printf("alphabet bytes: %ld\n\n", sizeof(alphabet));   // 27 bytes (26 + '\0')
    printf("%s\n", greetings);
    printf("%s\n", str);
    printf("%c\n", greetings[1]);
    str[0] = 'J';
    printf("%s\n", str);
}

/**
 * Copies the string from src to dest
 * Note that the size of dest should be large enough to store the result of the two strings combined
 */
void copy() {
    printf("\n---String Copy---\n");
    char src[6] = "hi", dest[6] = "empty";

    printf("Initial source string: %s\n", src);
    printf("Initial destination string: %s\n", dest);

    // Copy bytes from src to dest
    strcpy(dest, src);

    printf("Final copied string: %s\n", dest);
}
