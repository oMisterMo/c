#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void shift();
void flags();

int main() {

    shift();
    flags();
    printf("\n");

    return 0;
}

/**
 * Left shift same as mult 2
 */
void shift() {
    printf("\n---Shift---\n");
    uint8_t number = 0b00000001 << 1;
    printf("%d\n", number);
}

void flags() {
    printf("\n---Flags---\n");
    // 1 byte represents 8 different states
    // 1 2 4 8 16 32 64 128

    // Init Flags
    uint8_t one = 1;
    uint8_t two = 2;
    uint8_t four = 4;
    uint8_t eight = 8;
    uint8_t sixteen = 16;
    uint8_t thirty_two = 32;
    uint8_t sixty_four = 64;
    uint8_t one_hundred_twenty_eight = 128;

    printf("%3d | %08b\n", one, one);
    printf("%3d | %08b\n", two, two);
    printf("%3d | %08b\n", four, four);
    printf("%3d | %08b\n", eight, eight);
    printf("%3d | %08b\n", sixteen, sixteen);
    printf("%3d | %08b\n", thirty_two, thirty_two);
    printf("%3d | %08b\n", sixty_four, sixty_four);
    printf("%3d | %08b\n", one_hundred_twenty_eight, one_hundred_twenty_eight);

    printf("\n");

    // Set Flags
    uint8_t flags = one | two | one_hundred_twenty_eight;
    printf("flags: %08b\n\n", flags);

    // Extract Flags
    bool isOneSet = flags & one;
    bool isTwoSet = flags & two;
    bool isFourSet = flags & four;

    printf("is one set: %d\n", isOneSet);
    printf("is two set: %d\n", isTwoSet);
    printf("is four set: %d\n", isFourSet);

}

void more_flags() {
    // ATTEMPT 1 [00000001]
    printf("%d\n", (1 << 0));
    printf("%d\n", (1 << 1));
    printf("%d\n", (1 << 2));
    printf("%d\n", (1 << 3));
    
    // ATTEMPT 2 [00000001]
    printf("============");
    printf("\n");
    for (int i = 0; i < 12; ++i) {
        printf("%d\n", (1 << i));
    }
    printf("============");
    printf("\n");
}
