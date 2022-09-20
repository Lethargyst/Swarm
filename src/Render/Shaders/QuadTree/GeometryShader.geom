#version 460

layout (points) in;
layout (line_strip, max_vertices = 5) out;
// layout (points, max_vertices = 5) out;

uniform vec2 resolution;
in vec4 info[];

void main() {        
    gl_Position = vec4(info[0].x, info[0].y, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0].x, info[0].y + info[0].w, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0].x + info[0].z, info[0].y + info[0].w, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0].x + info[0].z, info[0].y, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0].x, info[0].y, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}  