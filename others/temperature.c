#include <stdio.h>

float to_celcius(float f);
float to_fahrenheit(float c);

int main() {
    
    printf("Temperature Conversion\n\n");

    printf("Celcius Fahrenheit\n");
    for (int i = -10; i < 20; i++) {
        float c = i * 2.0;
        printf("%6.1f°C %6.2f°F\n", c, to_fahrenheit(c));
    }
    printf("\n");

    printf("Fahrenheit Celcius\n");
    for (int i = 0; i < 20; i++) {
        float f =  i * 4;
        printf("%6.2f°F %6.1f°C\n", f, to_celcius(f));
    }

    printf("\n");
    return 0;
}


float to_celcius(float f) {
    // return (5 / 9) * (f - 32); // Integer division truncates the result
    return 5.0 * (f - 32.0) / 9.0;
}

float to_fahrenheit(float c) {
    return c * 9.0 / 5.0 + 32.0;
}
