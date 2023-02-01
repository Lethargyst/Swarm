#version 330
layout (location = 0) in vec2 inPos;  

void main()
{
    vec2 pos = vec2(inPos.x * 2 - 1, inPos.y * 2 - 1);
    gl_Position = vec4(pos, 0.0, 1.0);
}       