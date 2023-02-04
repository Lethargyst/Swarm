#version 330

precision mediump float;

in vec3 gColor;
uniform vec2 resolution;
out vec4 fragColor;

void main() { 
	fragColor = vec4(gColor.x / 255, gColor.y / 255, gColor.z / 255, 1.0);
	// fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
