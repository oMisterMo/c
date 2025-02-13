#include <stdio.h>      // printf
#include <stdbool.h>    // bool
#include <assert.h>     // assert()
#include <stdint.h>     // uintptr_t
#include <stdlib.h>     // rand()

#include "object_pool.h"

typedef struct PoolObject {
    Vector2 obj;        // 8 bytes
    struct PoolObject *next;
    int index;          // testing only
} PoolObject;

// Allocate some objects
PoolObject object_pool[NUM_OBJECTS] = { 0 };
PoolObject *head = NULL;

// __attribute__((construtor)) void InitializePool() {

/**
 * Set the next value of each node
 * Set the last node to point to NULL
 * Set the first node
 */
void InitializePool() {
    printf("============== init ============= \n\n");
    for (int i = 0; i < NUM_OBJECTS - 1; ++i) {
        object_pool[i].next = &(object_pool[i + 1]);
        object_pool[i].index = i;   // test
    }
    
    object_pool[NUM_OBJECTS - 1].next = NULL;
    object_pool[NUM_OBJECTS - 1].index = NUM_OBJECTS -1; // test
    head = object_pool;
    // head = &(object_pool[0]);
}

/**
 * Get a free objet O(1) - Malloc
 */
Vector2 *BorrowVector2(void) {

    if (head) {
        // Remove the first node from the list
        PoolObject *result = head;
        head = head->next;
        // Return the head
        printf("%c [%d] %p\n", BORROW, result->index, &(result->obj));
        return &(result->obj);
    }



    printf("Error - No free objects.\n");
    return NULL;
}

/**
 * Release the object that is longer in use - Free
 * Most recently used is set to the front
 */
void ReturnVector2(Vector2 *v) {
    // Calculate the index of the object to return
    unsigned int i = ((uintptr_t) v - (uintptr_t) object_pool) / sizeof(PoolObject);

    // Make sure this is a real pointer at the computed memory address i
    assert(&(object_pool[i].obj) == v);

    // Add to the front of the list
    PoolObject *object_to_return = &(object_pool[i]);
    object_to_return->next = head;
    head = object_to_return;


    // Return the object to the pool
    printf("%c [%d] %p\n", RELEASE, object_to_return->index, v);
    return;
}

int main() {

    printf("------------\n");
    printf("Object Pool (FAST - free list)\n");
    printf("------------\n");
    printf("8 bytes = Vector2 { int x, int y }\n");
    printf("4 bytes = bool allocated\n");
    printf("\n");
    printf("Total pool objects: %d\n", NUM_OBJECTS);
    printf("Total size of pool: %ld bytes\n\n", sizeof(object_pool));

    InitializePool();

    // Stress test...
    for (int i = 0; i < NUM_ROUNDS; ++i) {
        // Borrow items
        int max = rand() % NUM_OBJECTS;
        printf("Start round %d    r:%d\n", i+ 1,  max);
        Vector2 *vectors[max];
        for (int j = 0; j < max; ++j) {
            vectors[j] = BorrowVector2();
        }
        // Do something with vectors

        // Free them
        for (int j = 0; j < max; ++j) {
            ReturnVector2(vectors[j]);
        }

        printf("\n");
    }


    return 0;
}
