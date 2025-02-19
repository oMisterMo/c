#include <stdio.h>

int main() {

    /**
     * 32-bit processor takes 4 bytes at a time.
     * 64-bit processor takes 8 bytes at a time.
     */

     // 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048

    typedef struct {
        int x;
        int y;
    } Vector2;

    typedef struct {
        int x;
        int y;
        int z;
    } Vector3;

    typedef struct {
        int age;        // 4
        char sex;       // 1
        char name[8];   // 8
    } Person;

    printf("Hello world!\n");

    Person mo = { 33, 'M', "Mohammed" };
    printf("%ld\n", sizeof(Vector2));
    printf("%ld\n", sizeof(Vector3));
    printf("%ld\n", sizeof(Person));
    printf("age: %d\n", mo.age);
    
    
    

    return 0;
}
