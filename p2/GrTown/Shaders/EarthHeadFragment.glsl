#version 400

out vec3 color;
in vec3 sun;
in vec3 normal;


void main (void) {
  //frag_colour = fragmentColor;
 //color = vec3(0,1,0);
  float  ambient = 0.3;

  float theta = clamp(dot(normal,sun),0,1);
  float light = theta * 2 + ambient;
  //color = light * vec3(1,0,0);
  color =  light * vec3(0.623,0.623,0.623);
};
  