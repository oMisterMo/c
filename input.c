#include <stdio.h>

void echo();
void single_char();
void single_int();
void multiple_input();
void scanf_string();    // single word
void fgets_string();    // multiple words
void count_lines();
void count_blanks_tabs_newlines();
void skip_additional_blanks();
void replace_special_characters();
void count_words();

#define IN 1        // Inside a word
#define OUT 0       // Outside a word

const char INPUT_STR[] = "%c ";
const char INPUT_CHAR = '$';

/**
 * Use the scanf() function to get a single word as input, and use fgets() for multiple words.
 */
int main() {

    
    // single_char();
    // single_int();
    // multiple_input();
    // scanf_string();
    // fgets_string();
    // count_lines();
    // count_blanks_tabs_newlines();
    // skip_additional_blanks();
    // replace_special_characters();
    count_words();

    return 0;
}

/**
 * This function reads characters from the standard input and writes them to the standard output.
 */
void echo() {
    // getchar() = unsigned int (characters on ascii table)
    // EOF = -1
    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }

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

void count_lines() {
    printf("Counting the number of new lines\n\n");
    printf("Type 'Ctrl + D' to exit\n");
    printf("Type 'Ctrl + C' to terminate\n");
    printf("\n");
    printf(INPUT_STR, INPUT_CHAR);

    int c, nl;
    nl = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            ++nl;
            printf("Number of lines: %d (ctrl + d to exit)\n", nl);
            printf(INPUT_STR, INPUT_CHAR);
        }
    }
    printf("Total lines: %d\n", nl);
}

void count_blanks_tabs_newlines() {
    printf("Counting blanks, tabs and newlines\n\n");
    printf("Type 'Ctrl + D' to exit\n");
    printf("Type 'Ctrl + C' to terminate\n");
    printf("\n");
    printf(INPUT_STR, INPUT_CHAR);

    int c, nl;
    nl = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n' || c == '\t' || c == ' ') {
            ++nl;

            printf("Number of lines: %d\n", nl);
            printf(INPUT_STR, INPUT_CHAR);
        }
    }
    printf("\b\b\b\n\nTotal count: %d\n", nl);
}

void skip_additional_blanks() {
    printf("A program to copy its input to its output, replacing each string of one or more blanks by a single blank.\n\n");
    printf("Type 'Ctrl + D' to exit\n");
    printf("Type 'Ctrl + C' to terminate\n");
    printf("\n");
    printf(INPUT_STR, INPUT_CHAR);

    int c, nl, nb;
    nb = 0;
    nl = 0;

    // a b c
    while ((c = getchar()) != EOF) {
        if ( c == ' ') {
            ++nb;

            if (nb > 1) {
                continue;
            }
            // printf("Spaces: %d\n", nb);
        }

        printf("%c", c);

        if ( c == '\n') {
            nb = 0;
        }
    }
    printf("\b\b\b\n\nTotal count: %d\n", nl);
}

void replace_special_characters() {
    printf("A program to copy its input to its output, replacing each tab by \\t each backspace by \\b, and each backslash by \\\\.\n\n");
    printf("Type 'Ctrl + D' to exit\n");
    printf("Type 'Ctrl + C' to terminate\n");
    printf("\n");
    printf(INPUT_STR, INPUT_CHAR);

    int c;

    while ((c = getchar()) != EOF) {
        if ( c == '\t') {
            printf("\\t");
        } else if (c == '\b') {
            printf("\\b");
        } else if (c == '\\') {
            printf("\\\\");
        } else {
            printf("%c", c);
        }
    }
}

/**
 * Exercise 1-11. How would you test the word count program? What kinds of input are most likely to uncover bugs if there are any?
 *
 * To test I would use the following inputs:
 * - a single word
 * - multiple words
 * - a single word with multiple spaces
 * - multiple words with multiple spaces
 * - a single word with a newline
 * - multiple words with a newline
 * - a single word with a tab
 * - multiple words with a tab
 * - a single word with a tab and a newline
 * - multiple words with a tab and a newline
 * - a single word with a tab and a space
 *
 * The kind of inputs that are most likely to uncover bugs are:
 * - special characters
 * - function characters
 * - non ascii characters
 * - emojis
 *
 */
void count_words() {
    printf("A program to count lines, words and characters.\n\n");
    printf("Type 'Ctrl + D' to exit\n");
    printf("Type 'Ctrl + C' to terminate\n");
    printf("\n");
    printf(INPUT_STR, INPUT_CHAR);
    int c, nl, nw, nc, state;

    state = OUT;
    nl = nw = nc = 0;

    while ((c = getchar()) != EOF) {
        ++nc;
        if (c == '\n') {
            ++nl;
        }
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
        } else if (state == OUT) {
            state = IN;
            ++nw;
        }

        // Exercise 1-12. Write a program that prints its input one word per line.
        if (state == IN) {
            printf("%c", c);
        } else {
            printf("\n");
        }
    }
    printf("--------------------\n");
    printf("lines %d\nwords %d\ncharacters %d\n", nl, nw, nc);
    printf("--------------------\n");
}
