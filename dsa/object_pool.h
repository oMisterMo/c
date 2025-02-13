#ifndef OB_POOL_H_
#define OB_POOL_H_

#define BORROW '+'
#define RELEASE '-'

#define NUM_OBJECTS 10
#define NUM_ROUNDS 10

typedef struct {
    int x;
    int y;
} Vector2;

Vector2 *BorrowVector2(void);
void ReturnVector2(Vector2 *v);



// Allocate some objects
// Vector2 object_pool[NUM_OBJECTS] = { 0 };
// PoolObject object_pool[NUM_OBJECTS] = { 0 };
// Reduce wasted bytes as a result of memory alignment
// typedef struct {
//     Vector2 obj[NUM_OBJECTS];
//     bool allocated[NUM_OBJECTS]; // No direct access
// } PoolObject;


#endif
