#include <stdio.h>

#define PI 3.14159
#define AREA_OF_CIRCLE(radius) (PI * radius * radius)

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

#define PRINT_INT(x) printf("%d\n", x)
#define PRINT_FLOAT(x) printf("%f\n", x)
#define PRINT_DOUBLE(x) printf("%lf\n", x)
#define PRINT_INT_PTR(x) printf("%p\n", x)
#define PRINT_FLOAT_PTR(x) printf("%p\n", x)
#define PRINT_DOUBLE_PTR(x) printf("%p\n", x)

// #define PRINT_INT_PTR_VAL(x) printf("%d\n", *x)
// #define PRINT_FLOAT_PTR_VAL(x) printf("%f\n", *x)
// #define PRINT_DOUBLE_PTR_VAL(x) printf("%lf\n", *x)
// #define PRINT_INT_PTR_ADDR(x) printf("%p\n", &x)
// #define PRINT_FLOAT_PTR_ADDR(x) printf("%p\n", &x)
// #define PRINT_DOUBLE_PTR_ADDR(x) printf("%p\n", &x)

/**
 * One way to deal with magic numbers is to give then a symbolic name.
 */
int main() {

    PRINT_INT(10);

    PRINT_FLOAT(PI);

    PRINT_INT(MAX(24, 3));

    return 0;
}
