#version 400

in vec3 vp;

out vec3 sun;
out vec3 normal;
out float lightIntensity;
out mat3 rotationMatrix;

layout(location = 3) in vec3 _normal;
uniform mat4 MVP;
uniform vec3 sunDirection;
uniform float light;
uniform mat3 ry;

void main (void) {
	gl_Position = MVP * vec4(vp, 1.0);

	sun = sunDirection;

	mat3 normalMatrix = mat3(MVP);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	normal = normalize(_normal * normalMatrix);

	//normal = _normal;
	lightIntensity = light;
	rotationMatrix = ry;
};