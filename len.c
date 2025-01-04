#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stdint.h> // uint32_t
#include <time.h> // time

int main() {
    int nums[] = {25, 50, 75, 100};

    // Total bytes in the array divided by the size of the first element
    int length = sizeof(nums) / sizeof(nums[0]);
    int i;

    printf("size of int : %lu\n", sizeof(int));
    printf("number of elements in the array: %d\n", length);
    printf("size of int nums[] {25, 50, 75, 100} : %lu\n", sizeof(nums));

    printf("\nint (4 bytes) * nums.length (4) = 16 bytes\n");

    // for (i = 0; i < length; i++) {
    //     printf("%d ", nums[i]);
    // }

    printf("\n");
    return 0;
}

