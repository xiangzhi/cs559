#version 400

in vec3 vp;
uniform mat4 MVP;
uniform vec3 sunDirection;
layout(location = 3) in vec3 _normal;
layout(location = 2) in vec2 _UV;

out vec3 normal;
out vec3 sun;
out vec2 uv;

void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);
	normal = _normal;
	uv = _UV;
	sun = sunDirection;
};