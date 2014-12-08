#version 400

in vec3 sun;
in vec3 normal;

out vec3 color;

void main (void) {
  //frag_colour = fragmentColor;

  float ambient = 0.3;
  float theta = clamp(dot(normal,sun),0,1);
  float light = theta + ambient;

  color = light * vec3(1,0,0);
};