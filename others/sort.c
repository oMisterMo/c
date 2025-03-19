#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NO_OF_ELEMENTS 5

// Callbacks
int descending (const void *a, const void *b) {
    // Cast void pointer -> Integer pointer
    // Then deference to get the value
    return *(int*) b -  *(int*) a;
}
int ascending (const void *a, const void *b) {
    return *(int*) a -  *(int*) b;
}

// General Purpose
void PopulateArray(int *arr, int len) {
    for (int i = 0; i < len; ++i) {
        arr[i] = rand() % 99;
    }
}
void PrintArray(int *arr, int len) {
    for (int i = 0; i < len - 1; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("%d", arr[len - 1]);
    printf("\n");
}

int main(void) {

    // Random seed
    srand(time(NULL));

    // Create simple array
    // int arr[NO_OF_ELEMENTS] = { 3, 6, 1, 0, 4};
    int arr[NO_OF_ELEMENTS];
    PopulateArray(arr, NO_OF_ELEMENTS);

    printf("==================\n");
    PrintArray(arr, NO_OF_ELEMENTS);
    printf("==================\n");

    // Sort descending
    qsort(arr, NO_OF_ELEMENTS, sizeof(arr[0]), descending);
    PrintArray(arr, NO_OF_ELEMENTS);

    // Sort ascending
    qsort(arr, NO_OF_ELEMENTS, sizeof(arr[0]), ascending);
    PrintArray(arr, NO_OF_ELEMENTS);


    printf("\n");
    // printf("Hell o MO\n");

    return 0;
}
