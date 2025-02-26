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


// Fragment (pixel) shader ✔️
// Vertex shader ❌

// Ran on the GPU
// A shader's sole purpose is to return four numbers: r, g, b,and a.
// Color is normalized [0 - 1];
// 0 = black
// 1 = white

// fragTexCoor is dependant on canvas/texture/what we are drawing
// ran on each pixel
void main()
{

    // vec2 uv = fragCoord / vec2(screenWidth, screenHeight);
    // finalColor = vec4(uv.x, uv.y, 0, 1);

    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    finalColor = vec4(x, y, 0, 1); // r, g, b, a

    // Get color from cpu
    // finalColor = fragColor;

    // Set new color
    // finalColor = vec4(0,1,0,1);

}

