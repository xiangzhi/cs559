#version 400

in vec3 vp;
uniform mat4 MVP;
uniform vec3 sunDirection;
layout(location = 3) in vec3 _normal;

out vec3 normal;
out vec3 sun;
out float opacity;

void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);
	//calculate opacity
	opacity = 1 - (-1 * vp.y/300);
	opacity = clamp(opacity,0,1);
	normal = _normal;
	sun = sunDirection;
};