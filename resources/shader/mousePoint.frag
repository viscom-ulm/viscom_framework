#version 330 core

in vec3 fragColor;
out vec4 color;

void main()
{
    //color = vec4(fragColor, 1.0);
   color = vec4(1.0);
   //vec2 pos = mod(gl_FragCoord.xy, vec2(50.0)) - vec2(25.0);
   //float dist_squared = dot(pos,pos);
   
   //color = (dist_squared < 400) ? vec4(.90, .90, .90, 1.0) : vec4(.20, .20, .40, 1.0);    
}