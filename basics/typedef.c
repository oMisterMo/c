/**
 * The typedef is a keyword that is used to provide existing data types with a new name.
 *
 *
 * examples...
 *
 * typedef <existing_type> <new_type>
 *
 * typedef int Integear
 * typedef "struct Point {}" Point
 *
 */

#include <stdio.h>

struct Vector2 {
    float x;
    float y;
};

// typedef struct Vector2 Vector2;  // Now you can just use Vector2

struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float a;
} Vector4;

// Create user defined type called "Five" which means "struct Vector5"
typedef struct Vector5 {
    float x;
    float y;
    float z;
    float a;
    float b;
} Five;

typedef int Integer;
typedef long long ll;

int main() {


    printf("\n");
    struct Vector2 v2 = { 0 };
    struct Vector3 v3 = { 0 };
    Vector4 v4 = { 0 };
    Five v5 = { 0 };    // same as struct Vector5 v5;
    Integer num = 5;
    ll num2 = 2222;

    printf("%f, %f, %f, %f, %d, %lld\n", v2.x, v3.x, v4.x, v5.x, num, num2);
    
    
    printf("\n");
    printf("MOoO\n");
    printf("\n");



    return 0;
}
