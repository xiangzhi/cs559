#version 400

in vec3 vp;
out vec4 pos;

uniform mat4 MVP;
void main (void) {
	pos = vec4(vp, 1.0);
	gl_Position = MVP * vec4(vp, 1.0);
};