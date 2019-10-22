#version 330 core

uniform vec3 inputColor;

in vec2 texCoord;

out vec4 color;

void main()
{
    color = vec4(inputColor, 1.0);
}