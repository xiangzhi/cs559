#version 400

in vec3 sun;
in vec3 normal;
in float lightIntensity;
in vec3 viewDirection;
in mat3 rotationMatrix;
in float camDistance;

out vec4 color;


//based on http://ruh.li/GraphicsPhongBlinnPhong.html
//and http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/code/WebGLShaderLightMat/ShaderLightMat.html
//a simple Blinn-Phong lighting
void main (void) {
 
  vec3 ambientColor = vec3(0.1,0.1,0.1);
  //color of material
  vec3 diffuseColor = vec3(0.4,0.4,0.4);
  //lower than 1,1,1 due to long distance from sun
  vec3 specularColor = vec3(1,1,1);

  float speculatIntensity = 0;
  float shininess = 2;

  //calculate diffuse lighting
  float diffuseIntensity = clamp(dot(normal, sun),0,1);

  if(dot(sun,normal) > 0){
  	vec3 halfway = normalize(sun + viewDirection);
  	float specTemp = max(dot(normal,halfway),0.0);
  	speculatIntensity = pow(specTemp,shininess);
  }

  vec3 tempLight = speculatIntensity * specularColor + diffuseIntensity * diffuseColor + ambientColor;
    //no sun at all.
  if(sun == vec3(0)){
  	//always dark
    tempLight = vec3(0,0,0);
  	tempLight = vec3(0.1,0.1,0.1) + vec3(0.5, 0.5, 0.5) * clamp( 1 - (abs(camDistance)/500),0,1);
  }
  color = vec4(tempLight,1.0);
};