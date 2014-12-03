#version 400

in vec3 vp;
out vec3 fragmentColor;

uniform mat4 MVP;
layout(location = 1) in vec3 vertexColor;

void main (void) {
	vec4 v = vec4(vp, 1.0);
	if(MVP[0][0] != 0){
		gl_Position = MVP * v;
	}
	else{
		gl_Position = v;
	}
	fragmentColor = vertexColor;
};