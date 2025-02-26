#include <stdio.h>
#include <math.h>

#define PI atan2f(1, 1) * 4

int main(void) {

    printf("%lf\n\n", PI);


    size_t samples = 8;

    // Sample size
    float in[samples];

    // Output measured in hz (0 hz, 1 hz, 2 hz, ... 8 hz)
    float out[samples];

    for (size_t i = 0; i < samples; ++i) {
        float t = (float) i / samples;
        // printf("t = %.2f, ", t);
        in[i] = sinf(2 * PI * t * 1) + sinf(2 * PI * t * 2);
    }
    for (size_t f = 0; f < samples; ++f) {
        out[f] = 0;

        for (size_t i = 0; i < samples; ++i) {
            float t = (float) i / samples;
            out[f] += in[i] * sinf( 2 * PI * f * t );
        }
    }

    // Print frequencies
    printf("%8s %6s\n", "Frequency", "Value");
    for (size_t f = 0; f < samples; ++f) {
        printf("%8lu ", f);
        printf("%6.2f\n", out[f]);
    }

    // If the value is positive, it is contained
    // +4 for each frequency found


    printf("\n\n");


    // Log it
    // for (size_t i = 0; i < samples; ++i) {
    //     printf("%.2f\n", in[i]);
    // }

    // printf("%lf\n", sin());
    printf("\n");


    return 0;
}
