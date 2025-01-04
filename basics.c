#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

/**
 * implicit = automatic
 * explicit = manual
 */
int main(int argc, char *argv[]) {
    // Types
    char a; // 1 byte
    char *e[10]; // string

    int b; // 4 bytes
    int8_t c; // 1 byte
    int16_t d; // 2 bytes
    int32_t f; // 4 bytes
    int64_t g; // 8 bytes
    uint8_t h; // 1 byte
    uint16_t i; // 2 bytes
    uint32_t j; // 4 bytes
    uint64_t k; // 8 bytes

    float l; // 4 bytes
    double m; // 8 bytes

    l = 3.14159;
    printf("%f\n", l); // 3.141590 (6 decimal places)
    printf("%.2f\n", l); // 3.14 (2 decimal places)

    h = 255; // uint8_t (0 - 255)
    printf("%d \n", h);

    // Arrays
    printf("*** Arrays ***\n");
    int arr[15];
    char list[3];

    arr[0] = 10;
    arr[1] = 2;
    arr[2] = 2;

    list[0] = 'a';
    list[1] = 'b';
    list[2] = 'c';

    printf("arr[0]= %d\n", arr[0]);
    printf("list[1]= %c\n", list[1]);

    // Loops
    printf("\n*** Loops ***\n");
    for(int i = 0; i < 10; i++) {
        printf("%d ", i);
    }

    // Others
    printf("\n\n*** Others ***\n");
    printf("char %lu\n", sizeof(char));
    printf("int %lu\n", sizeof(int));
    printf("float %lu\n", sizeof(float));
    printf("double %lu\n", sizeof(double));

    printf("uint8_t %lu\n", sizeof(uint8_t));


    printf("\n");
    return 0;
}