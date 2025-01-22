#include <stdio.h>

struct Vector2 {
    float x;
    float y;
};

struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct Vector4 {
    int vars;
} Vector4;

// Create user defined type called "Five" which means "struct Vector5"
typedef struct Vector5 {
    int vars;
} Five;

typedef int Integer;
typedef long long ll;

/**
 * The typedef is a keyword that is used to provide existing data types with a new name.
 *
 * typedef <existing_type> <new_type>
 *
 */
int main() {


    struct Vector2 v2 = { 0 };
    struct Vector3 v3 = { 0 };
    Vector4 v4 = { 0 };
    Five v5 = { 0 };    // same as struct Vector5 v5;
    Integer num = 5;
    ll num2 = 2222;


    printf("MOoO\n");



    return 0;
}
