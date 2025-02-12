
/**
 * null is a value, 0 is a number, and '\0' is a character.
 * 
 * https://stackoverflow.com/questions/1296843/what-is-the-difference-between-null-0-and-0
 */
int main() {


    // The null character
    printf("'%d' '%c'\n", '\0', '\0');  // '0' ''
    // The number 0
    printf("'%d' '%c'\n", '0', '0');    // '48' '0'


    return 0;
}
