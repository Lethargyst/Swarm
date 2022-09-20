#version 460
layout (location = 0) in vec2 inPos;  
layout (location = 1) in vec2 inSize;  

uniform vec2 resolution;
out vec4 info;

void main()
{
    vec2 pos = vec2(inPos.x / resolution.x  * 2 - 1, inPos.y / resolution.y * 2 - 1);
    vec2 size = vec2(inSize.x / resolution.x  * 2 - 1, inSize.y / resolution.x  * 2 - 1);
    info = vec4(pos.x, pos.y, size.x, size.y);
}       