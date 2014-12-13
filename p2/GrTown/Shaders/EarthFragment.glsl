#version 400


// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureInput;
uniform sampler2D normalInput;

uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3;
uniform vec3 sunDirection;

void main(){

// Light emission properties
// You probably want to put them as uniforms
vec3 LightColor = vec3(1,1,1);
float LightPower = 40.0;

// Material properties
vec3 MaterialDiffuseColor = texture( textureInput, UV ).rgb;
vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
vec3 MaterialSpecularColor = vec3(1,1,1);

// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
vec3 n = normalize(texture( normalInput, UV).rgb*2.0 - 1.0);

//get the nromal of light direction
vec3 l = normalize(LightDirection_tangentspace);
float cosTheta = clamp( dot( n,sunDirection ), 0,1 );
// Eye vector (towards the camera)
vec3 E = normalize(EyeDirection_tangentspace);
// Direction in which the triangle reflects the light
vec3 R = reflect(-l,n);
//calculate the diffuse intensity
float cosAlpha = clamp( dot( E,R ), 0,1 );

vec3 tempcolor = MaterialAmbientColor  + MaterialDiffuseColor *
 cosTheta + MaterialSpecularColor * LightColor 
 * LightPower * pow(cosAlpha,5);

 color = vec4(tempcolor,1.0);

}

