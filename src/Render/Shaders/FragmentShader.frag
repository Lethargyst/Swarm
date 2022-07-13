#version 330 core
precision mediump float;
in vec3 pos;
out vec4 FragColor;

const int cycles = 6;

void main() {
    vec2 uv = gl_FragCoord.xy / 30.0;
    vec3 color = vec3(2.0, 1.0, 1.0);
    for(int i=0; i < cycles; i++) {
        color = pow(vec3(dot(sin(uv),pos.xy), dot(cos(uv),pos.xy), dot(atan(uv),pos.xy)), color);
    };
	FragColor = vec4(color, 1.0);
}
