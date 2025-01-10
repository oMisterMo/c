#include <stdio.h>

void single_char();
void single_int();
void multiple_input();
void scanf_string();    // single word
void fgets_string();    // multiple words

const char INPUT_STR[] = "%c ";
const char INPUT_CHAR = '$';

/**
 * Use the scanf() function to get a single word as input, and use fgets() for multiple words.
 */
int main() {

    
    single_char();
    // single_int();
    // multiple_input();
    // scanf_string();
    // fgets_string();

    return 0;
}

void single_char() {

    printf("Enter a character: \n");
    printf(INPUT_STR, INPUT_CHAR);

    char c = getchar();

    // putchar(c);  // prints the character, but not the newline

    printf("You entered: %c\n", c);
}
/**
 * Single word input
 */
void single_int() {
    int muNum;

    printf("Enter a number: \n");
    printf(INPUT_STR, INPUT_CHAR);

    scanf("%d", &muNum);

    printf("You entered: %d\n", muNum);
}

void multiple_input() {
    // Create an int and a char variable
    int myNum;
    char myChar;

    // Ask the user to type a number AND a character
    printf("Type a number AND a character and press enter: \n");
    printf(INPUT_STR, INPUT_CHAR);

    // Get and save the number AND character the user types
    scanf("%d %c", &myNum, &myChar);

    // Print the number
    printf("Your number is: %d\n", myNum);

    // Print the character
    printf("Your character is: %c\n", myChar);
}

/**
 * The scanf() function has some limitations:
 * it considers space (whitespace, tabs, etc) as a terminating character,
 * which means that it can only display a single word
 */
void scanf_string() {
    char firstName[20];

    printf("Enter your first name: \n");
    printf(INPUT_STR, INPUT_CHAR);

    // you don't have to use the reference operator (&) for strings
    scanf("%s", firstName);

    printf("Hello %s!\n", firstName);
}

void fgets_string() {
    char fullName[30];

    printf("Enter your full name: \n");
    printf(INPUT_STR, INPUT_CHAR);
    fgets(fullName, 30, stdin); // sizeof(fullName); or 30

    printf("Hello %s\n", fullName);
}
