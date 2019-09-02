#version 330 core

uniform int hitCircle;
uniform int circleSize;
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

   //color = vec4(1.0);
   //vec2 pos = mod(gl_FragCoord.xy, vec2(50.0)) - vec2(25.0);
   //float dist_squared = dot(pos,pos);
   
   //color = (dist_squared < 400) ? vec4(.90, .90, .90, 1.0) : vec4(.20, .20, .40, 1.0);    
}