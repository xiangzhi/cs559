#version 400

in vec3 vp;
out vec2 UV;

layout(location = 1) in vec2 vertexUV;
uniform mat4 MVP;
void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);
	UV = vertexUV;
};