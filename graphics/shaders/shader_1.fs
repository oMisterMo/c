#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;   // x, y, same as fragCoord
in vec4 fragColor;      // input color from cpu

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// variables
uniform int screenWidth;                    // Width of the screen
// const uniform float screenHeight;        // Height of the screen


uniform float seconds;

// fragTexCoor is dependant on canvas/texture/what we are drawing
// ran on each pixel
void main()
{

    float frequency = 1.0f;
    float color = sin ( seconds );

    // frequency = frequency + 0.1;
    // if (frequency >= 1.0) frequency = 0;

    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    finalColor = vec4(x, frequency, color, 1); // r, g, b, a

}

