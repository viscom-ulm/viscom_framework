#version 330 core

uniform int hitCircle;
uniform float circleSize;
uniform float circleRatio;

out vec4 color;


void main()
{
    color = vec4(1.0, 0.75, 0.0, 1.0);

    if(hitCircle == 1) color = vec4(0.0, 1.0, 1.0, 1.0);

    vec2 pos = 2.0 * gl_PointCoord - 1.0;
	pos.x = pos.x / circleRatio;
    float rad = dot(pos, pos);
    if(rad > 1.0 || rad < 1.0 - 12.0 / circleSize) discard;
}