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

void draw_mango();
void diagonal_lines_from_bottom_left();
void verticle_moving_lines();

void main()
{
    // draw_mango();
    // diagonal_lines_from_bottom_left();
    verticle_moving_lines();
}

void draw_mango() {
    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    finalColor = vec4(x, y, 0, 1); // r, g, b, a
}

void diagonal_lines_from_bottom_left() {
    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    float frequency = 10;
    float color = sin( x / y * frequency + seconds) * y;
    finalColor = vec4(color, 0, 0, 1); // r, g, b, a
}

void verticle_moving_lines() {
    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    float frequency = 10;
    float color = sin( x * frequency + seconds) * y;
    finalColor = vec4(color, 0, 0, 1); // r, g, b, a
}
