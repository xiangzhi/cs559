#version 400

in vec3 vp;
uniform mat4 MVP;
uniform vec3 sunDirection;
layout(location = 3) in vec3 _normal;

out vec3 normal;
out vec3 sun;

void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);
	normal = _normal;
	sun = sunDirection;
};