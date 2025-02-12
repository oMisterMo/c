#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>

#define BORROW '+'
#define RELEASE '-'

#define NUM_OBJECTS 10

typedef struct {
    int x;
    int y;
} Vector2;

typedef struct {
    Vector2 obj;    // 8 bytes
    bool allocated; // 4 bytes (No direct access)
} PoolObject;

// Reduce wasted bytes as a result of memory alignment
// typedef struct {
//     Vector2 obj[NUM_OBJECTS];
//     bool allocated[NUM_OBJECTS]; // No direct access
// } PoolObject;

// Allocate some objects
// Vector2 object_pool[NUM_OBJECTS] = { 0 };
PoolObject object_pool[NUM_OBJECTS] = { 0 };

/**
 * Get a free objet O(N)
 *
 *  Todo: Manage the free objects as a singly linked list (as a free list), instead of the allocated flag, now the allocation would be O(1)
 */
Vector2 *BorrowVector2(void) {
    // Loop through all objects, return the first free obj
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        if (object_pool[i].allocated == false) {
            object_pool[i].allocated = true;
            printf("%c [%d] %p\n", BORROW, i, &(object_pool[i].obj));
            return &(object_pool[i].obj);
        }
    }
    printf("Error - No free objects.");
    return NULL;
}
/**
 * Release the object that is longer in use (FASTER)
 */
void ReturnVector2(Vector2 *v) {
    // Calculate the index of the object to return
    unsigned int i = ((uintptr_t) v - (uintptr_t) object_pool) / sizeof(PoolObject);

    // Make sure this is a real pointer at the computed memory address i
    assert(&(object_pool[i].obj) == v);
    assert(object_pool[i].allocated);

    // Return the object to the pool
    object_pool[i].allocated = false;
    printf("%c [%d] %p\n", RELEASE, i, v);
    return;
}

// /**
//  * Release the object that is longer in use (SLOW)
//  */
// void ReturnVector2(Vector2 *v) {
//     for (int i = 0; i < NUM_OBJECTS; ++i) {
//         if (&(object_pool[i].obj) == v) {
//             assert(object_pool[i].allocated);
//             object_pool[i].allocated = false;
//             printf("%c [%d] %p\n", RELEASE, i, v);
//             return;
//         }
//     }
//     assert(false);  // This is a bug, look into it.
// }

int main() {

    printf("------------\n");
    printf("Object Pool\n");
    printf("------------\n");
    printf("Total size of pool: %ld bytes\n\n", sizeof(object_pool));

    Vector2 *v = BorrowVector2();
    Vector2 *v2 = BorrowVector2();
    ReturnVector2(v2);
    Vector2 *v3 = BorrowVector2();
    Vector2 *v4 = BorrowVector2();
    Vector2 *v5 = BorrowVector2();


    return 0;
}
