#version 330 core

uniform sampler2D inputTexture;

in vec2 texCoord;

out vec4 color;

void main()
{
    color = texture(inputTexture, texCoord);
}