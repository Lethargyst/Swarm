#version 460
layout (location = 0) in vec2 inPos;  

uniform vec2 resolution;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
}       