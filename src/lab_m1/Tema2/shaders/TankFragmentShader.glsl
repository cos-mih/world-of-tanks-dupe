#version 330

// Input
in vec3 color;
in vec3 damage;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 damagedColor = color - 0.2 * damage[0] * color;
    out_color = vec4(damagedColor, 1);
}
