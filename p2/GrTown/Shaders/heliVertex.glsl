#version 400

in vec3 vp;

out vec3 sun;
out vec3 normal;
out vec3 viewDirection;
out float lightIntensity;
out mat3 rotationMatrix;
out float camDistance;

layout(location = 3) in vec3 _normal;
uniform mat4 MVP;
uniform vec3 sunDirection;
uniform float light;
uniform mat3 ry;
uniform mat4 model;
uniform mat4 view;

void main (void) {
	//the position of the vertex in world space
	gl_Position = MVP * vec4(vp, 1.0);

	mat4 viewModel = view * model;
	vec3 viewDir = vec3(viewModel * vec4(vp, 1.0));
	viewDirection = normalize(-viewDir);

	//get camera position
	vec3 camPos = vec3(view[0][3],view[1][3],view[2][3]);
	camDistance = distance(vec3(gl_Position),camPos);

	//modify normal vector such that it remain the same sunDirection
	//even if it faces other direction due to rotation
	mat3 normalMatrix = mat3(model);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	normal = normalize(_normal * normalMatrix);

	//normal = _normal;
	lightIntensity = light;
	sun = sunDirection;
	rotationMatrix = ry;
};