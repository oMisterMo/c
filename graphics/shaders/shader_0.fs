#version 330

#define t seconds
#define r resolution.xy


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
void mouse_input();
void mo();

void main()
{
    // draw_mango();
    // diagonal_lines_from_bottom_left();
    // verticle_moving_lines();
    mouse_input();
    // mo();
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

void mouse_input() {
    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    float frequency = mouse.x / mouse.y;
    float color = sin( x * frequency ) * y;
    finalColor = vec4(color, 0, 0, 1); // r, g, b, a
}

// vec3 sdfCircle(vec2 uv, float r, vec2 offset) {
//   float x = uv.x - offset.x;
//   float y = uv.y - offset.y;

//   float d = length(vec2(x, y)) - r;

//   return d > 0 ? vec3(1) : vec3(0, 0, 1);
// }

void mo() {
    vec2 uv = fragTexCoord/resolution.xy; // <0,1>
    vec3 col = vec3(0); // start with black
    if (uv.x > 0.5) col = vec3(1); // make the right half of the canvas white

    // Output to screen
    finalColor = vec4(col, 1.0); // output color
}
