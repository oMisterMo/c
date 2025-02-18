#include <stdio.h>

/**
 * An enum is a special type that represents a group of constants (unchangeable values).
 */
int main(void) {

    enum Day {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday}; // 0, 1, 2, 3, 4, 5, 6
    enum Day today = Wednesday; // 2

    enum Level {
        LOW = 10,
        MEDIUM = 50,
        HIGH = 100,
        HIGHEST
    }; // 10, 50, 100, 101 (increments by 1 if not specified)

    printf("Day %d\n", today);
    printf("Level %d\n", MEDIUM);

    printf("\n");
    printf("sizeof(today): %ld\n", sizeof(today));

    return 0;
}
