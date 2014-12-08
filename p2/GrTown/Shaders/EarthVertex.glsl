#version 400

in vec3 vp;

layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;


out vec2 UV;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

uniform mat4 M;
uniform mat3 M3;
uniform mat4 V;
uniform mat4 P;

void main (void) {

	vec4 vector = vec4(vp, 1.0);
	gl_Position = (P * V * M) * vector;

	Position_worldspace = (M * vector).xyz;

	vec3 vertexPosition_cameraspace = (V * M * vector).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	UV = vertexUV;

	// model to camera = ModelView
	vec3 vertexTangent_cameraspace = M3 * tangent;
	vec3 vertexBitangent_cameraspace = M3 * biTangent;
	vec3 vertexNormal_cameraspace = M3 * normal;

	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirection_tangentspace = TBN * LightDirection_cameraspace;
	EyeDirection_tangentspace = TBN * EyeDirection_cameraspace;

};