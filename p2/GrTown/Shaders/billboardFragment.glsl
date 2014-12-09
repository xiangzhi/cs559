#version 400

in vec3 sun;
in vec2 UV;
in vec3 normal;
in float lightIntensity;
out vec3 color;

uniform sampler2D textureInput;
uniform float light;
void main (void) {
  //frag_colour = fragmentColor;

  float ambient = 0.3;
  float theta = clamp(dot(normal,sun),0,1);
  //float light = theta * 2 + ambient;

  color = light * texture(textureInput,UV).rgb;
  //color = vec3(1,0,0);
};