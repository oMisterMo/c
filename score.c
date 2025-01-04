#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stdint.h> // uint32_t
#include <time.h> // time

int main() {
    // Set the maximum possible score in the game to 500
    int maxScore = 500;

    // The actual score of the user
    int userScore = 423;

    /* Calculate the percantage of the user's score in relation to the maximum available score.
    Convert userScore to float to make sure that the division is accurate */
    float percentage = (float) userScore / maxScore * 100.0;

    // Print the percentage
    printf("User's percentage is %.2f", percentage);

    printf("\n");
    return 0;
}

