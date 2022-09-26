#version 460

layout (points) in;
layout (line_strip, max_vertices = 10) out;
// layout (points, max_vertices = 5) out;

uniform vec2 resolution;
in vec4 info[];

void main() {        
    gl_Position = vec4(info[0][0], info[0][1], 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0][0], info[0][1] + info[0][3], 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0][0] + info[0][2], info[0][1] + info[0][3], 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0][0] + info[0][2], info[0][1], 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(info[0][0], info[0][1], 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}  