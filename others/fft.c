#include <stdio.h>
#include <math.h>

#define PI atan2f(1, 1) * 4
size_t samples = 8;
// size_t samples = 4096 * 2;

enum SAMPLE_SIZE {
    SAMEPLE_LOW = 256,
    SAMEPLE_MEDIUM = 512,
    SAMEPLE_HIGH = 1024,
    SAMEPLE_HIGHER = 2048,
    SAMEPLE_HIGHEST = 4096,
};

void dft(float in[], float out[]) {
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
}

int main(void) {

    printf("%lf\n\n", PI);


    // Sample size
    float in[samples];

    // Output measured in hz (0 hz, 1 hz, 2 hz, ... 8 hz)
    float out[samples];

    dft(in, out);

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
