#version 400

out vec3 frag_color;
in vec3 fragmentColor;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSample;

void main (void) {
  frag_color = fragmentColor;
  //frag_color = texture(textureSample, UV).rgb;
};