#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;   // fragCoord
in vec4 fragColor;      // input color from cpu

// Output fragment color
out vec4 finalColor;

// uniforms
uniform vec2 resolution;
uniform float seconds;
uniform vec2 mouse;
uniform vec4 bands; // FFT results


// variables

void drawMango();

void main()
{
    drawMango();
}

void drawMango() {
    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    finalColor = vec4(x, y, 0, 1); // r, g, b, a
}
