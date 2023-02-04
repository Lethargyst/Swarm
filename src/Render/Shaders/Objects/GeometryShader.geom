#version 330

#define M_PI 3.1415926535897932384626433832795

layout (points) in;
layout (triangle_strip, max_vertices = 100) out;

in vec3 vColor[];
out vec3 gColor;

uniform vec2 resolution;

void drawCircle(vec2 pos, float size)
{
    vec2 length = vec2(size) / resolution.xy;
    int vertNum = 50;
    float alpha = 2 * M_PI / vertNum;

    for (int i = 0; i <= vertNum; i += 2)
    {
        gl_Position = vec4(pos, 0.0, 1.0);
        gColor = vColor[0];
        EmitVertex();  

        gl_Position = vec4(pos.x + (length.x * cos(alpha * i)),
                           pos.y + (length.y * sin(alpha * i)),
                           0.0, 1.0); 
        gColor = vColor[0];
        EmitVertex();   

        gl_Position = vec4(pos.x + (length.x * cos(alpha * (i + 1))),
                           pos.y + (length.y * sin(alpha * (i + 1))),
                           0.0, 1.0);
        gColor = vColor[0];
        EmitVertex();   
    }
    EndPrimitive();
}

void main() {    
    drawCircle(gl_in[0].gl_Position.xy, gl_in[0].gl_PointSize);
}  