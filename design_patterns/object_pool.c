#include <stdio.h>      // printf
#include <stdbool.h>    // bool
#include <assert.h>     // assert()
#include <stdint.h>     // uintptr_t
#include <stdlib.h>     // rand()

#include "object_pool.h"

typedef struct {
    Vector2 obj;        // 8 bytes
    bool allocated;     // 4 bytes (No direct access)
} PoolObject;

// Allocate some objects
PoolObject object_pool[NUM_OBJECTS] = { 0 };

/**
 * Get a free objet O(N) - Malloc
 */
Vector2 *BorrowVector2(void) {
    // Loop through all objects, return the first free obj
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        if (object_pool[i].allocated == false) {
            object_pool[i].allocated = true;
            // printf("%c [%d] %p\n", BORROW, i, &(object_pool[i].obj));
            return &(object_pool[i].obj);
        }
    }
    printf("Error - No free objects.");
    return NULL;
}

/**
 * Release the object that is longer in use (LESS SLOW) - Free
 */
void ReturnVector2(Vector2 *v) {
    // Calculate the index of the object to return
    unsigned int i = ((uintptr_t) v - (uintptr_t) object_pool) / sizeof(PoolObject);

    // Make sure this is a real pointer at the computed memory address i
    assert(&(object_pool[i].obj) == v);
    assert(object_pool[i].allocated);

    // Return the object to the pool
    object_pool[i].allocated = false;
    // printf("%c [%d] %p\n", RELEASE, i, v);
    return;
}

/**
 * Release the object that is longer in use (SLOW) - Free
 */
void ReturnVector2Slow(Vector2 *v) {
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        if (&(object_pool[i].obj) == v) {
            assert(object_pool[i].allocated);
            object_pool[i].allocated = false;
            // printf("%c [%d] %p\n", RELEASE, i, v);
            return;
        }
    }
    assert(false);  // This is a bug, look into it.
}

int main() {

    printf("------------\n");
    printf("Object Pool\n");
    printf("------------\n");
    printf("8 bytes = Vector2 { int x, int y }\n");
    printf("4 bytes = bool allocated\n");
    printf("\n");
    printf("Total pool objects: %d\n", NUM_OBJECTS);
    printf("Total size of pool: %ld bytes\n\n", sizeof(object_pool));


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
