#version 460 

precision mediump float;

in vec2 pos;

uniform vec2 resolution;


void main(){

    vec3 color = vec3(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y, 1.0);
	gl_FragColor = vec4(color, 1.0);
}