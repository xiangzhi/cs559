#version 400

in vec3 vp;

uniform mat4 MVP;
void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);
};