#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NO_OF_ELEMENTS 5

// Callbacks
int descending (const void *a, const void *b) {
    // There are no generic types
    // So you have to jump through extra hoops

    // 1. Cast void pointer -> Integer pointer
    // 2. Deference to get the value
    return *(int*) b -  *(int*) a;
}
int ascending (const void *a, const void *b) {
    return *(int*) a -  *(int*) b;
}

// General Purpose
bool IsEmpty(char *title) {
    return (title == NULL || title == "");
}
void PopulateArray(int *arr, int len) {
    for (int i = 0; i < len; ++i) {
        arr[i] = rand() % 99;
    }
}
void PrintArray(int *arr, int len, char *title) {
    printf("========= %s =========\n", IsEmpty(title) ? "Print Array" : title);
    for (int i = 0; i < len - 1; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("%d", arr[len - 1]);
    printf("\n\n");
}

int main(void) {

    // Random seed
    srand(time(NULL));

    // Create simple array
    // int arr[NO_OF_ELEMENTS] = { 3, 6, 1, 0, 4};
    int arr[NO_OF_ELEMENTS];
    PopulateArray(arr, NO_OF_ELEMENTS);

    // printf("==================\n");
    PrintArray(arr, NO_OF_ELEMENTS, "Original");
    // printf("==================\n");

    // Sort descending
    qsort(arr, NO_OF_ELEMENTS, sizeof(arr[0]), descending);
    PrintArray(arr, NO_OF_ELEMENTS, "Descending");

    // Sort ascending
    qsort(arr, NO_OF_ELEMENTS, sizeof(arr[0]), ascending);
    PrintArray(arr, NO_OF_ELEMENTS, "Ascending");


    printf("\n");
    // printf("Hell o MO\n");

    return 0;
}
