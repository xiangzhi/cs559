#version 400

out vec3 color;
in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D textureInput;
void main (void) {
  //frag_colour = fragmentColor;
 //color = vec3(0,1,0);
  float  ambient = 0.3;
  color = texture(textureInput, UV).rgb;
};