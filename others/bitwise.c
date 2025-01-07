#include <stdio.h>
#include <stdint.h>

int main() {

    uint8_t number = 0b00000001 << 1;
    printf("%d\n", number);

    return 0;
}
