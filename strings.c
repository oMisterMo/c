#include <stdio.h>
#include <string.h>

int main() {
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

    return 0;
}
