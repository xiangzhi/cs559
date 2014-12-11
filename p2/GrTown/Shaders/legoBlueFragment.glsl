#version 400

in vec3 sun;
in vec3 normal;
in float lightIntensity;
in mat3 rotationMatrix;

out vec3 color;

// Values that stay constant for the whole mesh.
void main (void) {
 
  //make sure normal facing the correct direction
  //vec3 newNormal = normal * rotationMatrix;
  //vec3 newSun = sun * rotationMatrix;
  float  ambient = 0.3;
  float theta = clamp(dot(normal, sun),0,1) * ambient;
  float light = theta * 2 + ambient;
  color =  light * vec3(0,0,1);
};