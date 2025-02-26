#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;   // x, y, same as fragCoord
in vec4 fragColor;      // input color from cpu


// Output fragment color
out vec4 finalColor;

void main()
{

    finalColor = vec4(1, 0, 0, 1); // r, g, b, a

}
