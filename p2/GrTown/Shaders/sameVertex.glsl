#version 400

in vec3 vp;
out vec2 UV;
out vec3 sun;
out vec3 normal;

layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 _normal;
uniform mat4 MVP;
uniform vec3 sunDirection;

void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);
	UV = vertexUV;
	sun = sunDirection;
	normal = _normal;
};