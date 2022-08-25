#version 460
layout (location = 0) in vec2 inPos;  
layout (location = 1) in float size;
layout (location = 2) in vec3 inColor;

out vec3 vColor;

uniform vec2 resolution;

void main()
{
    gl_PointSize = size;
    vec2 pos = vec2(inPos.x / resolution.x  * 2 - 1, inPos.y / resolution.y * 2 - 1);
    gl_Position = vec4(pos, 0.0, 1.0);
    vColor = inColor;
}       