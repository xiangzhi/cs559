#version 400

in vec3 sun;
in vec3 normal;
in vec2 uv;

out vec3 color;


uniform sampler2D textureInput;
void main (void) {
  //frag_colour = fragmentColor;

  float ambient = 0.3;
  float theta = clamp(dot(normal,sun),0,1);
  float light = theta * 2 + ambient;
  vec2 nuv = uv * 10;
  color = light * texture(textureInput, nuv).rgb;

};