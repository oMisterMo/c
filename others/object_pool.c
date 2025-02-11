#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define BORROW '+'
#define RELEASE '-'


typedef struct {
    int x;
    int y;
} Vector2;

typedef struct {
    bool allocated; // No direct access
    Vector2 obj;
} PoolObject;

// Allocate some objects
#define NUM_OBJECTS 10
// Vector2 object_pool[NUM_OBJECTS] = { 0 };
PoolObject object_pool[NUM_OBJECTS] = { 0 };

/**
 * Get a free objet
 */
Vector2 *BorrowVector2(void) {
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
 * Release the object, no longer in use
 */
void ReturnVector2(Vector2 *v) {
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        if (&(object_pool[i].obj) == v) {
            assert(object_pool[i].allocated);
            object_pool[i].allocated = false;
            printf("%c [%d] %p\n", RELEASE, i, v);
            return;
        }
    }
    assert(false);  // This is a bug, look into it.
}

int main() {

    printf("------------\n");
    printf("Object Pool\n");
    printf("------------\n");
    printf("%ld bytes\n", sizeof(object_pool));

    Vector2 *v = BorrowVector2();
    Vector2 *v2 = BorrowVector2();
    ReturnVector2(v2);
    Vector2 *v3 = BorrowVector2();
    Vector2 *v4 = BorrowVector2();
    Vector2 *v5 = BorrowVector2();


    return 0;
}
