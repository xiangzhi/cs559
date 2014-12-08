#version 400

in vec3 vp;
uniform mat4 MVP;
uniform mat4 trans;
uniform vec3 sunDirection;
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 center;
uniform vec3 size;
layout(location = 3) in vec3 _normal;
layout(location = 2) in vec2 _uv;

out vec3 normal;
out vec3 sun;
out vec2 UV;

void main (void) {

	vec4 finalPos = vec4(vp,1.0) * trans;

	gl_Position = MVP * finalPos;
	normal = _normal;
	sun = sunDirection;
	UV = _uv;
};