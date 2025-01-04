#include <stdio.h>
#include <string.h>

void string_copy();
void string_init();

int main() {

    string_init();
    string_copy();


    printf("\n");
    return 0;
}

void string_init() {
    printf("\n---String Init---\n");
    // string literal (automatically appends '\0')
    char greetings[] = "Hello World!";
    // character array
    char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // 26 characters


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
void string_copy() {
    printf("\n---String Copy---\n");
    char src[6] = "hi", dest[6] = "empty";

    printf("Initial source string: %s\n", src);
    printf("Initial destination string: %s\n", dest);

    strcpy(dest, src);
    printf("Final copied string: %s\n", dest);
}
