#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;   // x, y aka as fragCoord
in vec4 fragColor;      // input color from cpu

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// variables
uniform int screenWidth;                    // Width of the screen
uniform int screenHeight;                 // Height of the screen


uniform float seconds;

void lerp_green_blue();
void lightning();

// fragTexCoor is dependant on canvas/texture/what we are drawing
// ran on each pixel
void main()
{

    // lerp_green_blue();
    // lightning();

}


void lerp_green_blue() {
    float frequency = 2.0;
    float color = sin ( seconds * frequency );

    if (color < 0) color*=-1;

    // Draw uv mango, doesn't work on shapes, must be a texture
    float x = fragTexCoord.x;
    float y = fragTexCoord.y;
    finalColor = vec4(x, 1., color, 1); // r, g, b, a
}

void lightning() {
    float frequency = 3.0;
    float color = sin ( seconds * frequency );
    if (color < 0.1) color = 0.1;

    finalColor = vec4(color, color, color, 1); // r, g, b, a
}
