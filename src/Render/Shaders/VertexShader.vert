#version 460 
layout (location = 0) in vec2 aPos;  

out vec2 pos;

uniform vec2 resolution;

void main()
{
    gl_PointSize = abs(aPos.x * 100) + 10;
    gl_Position = vec4(aPos, 0.0, 1.0);
    pos = aPos;
}       