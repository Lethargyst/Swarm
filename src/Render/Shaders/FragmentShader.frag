#version 460

precision mediump float;

in vec3 gColor;

uniform vec2 resolution;

void main() { 
	gl_FragColor = vec4(gColor.x / 255, gColor.y / 255, gColor.z / 255, 1.0);
	// gl_FragColor = vec4(1.0);
}