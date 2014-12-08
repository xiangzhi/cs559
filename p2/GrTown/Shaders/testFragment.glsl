#version 400

out vec3 color;
in vec4 pos;

void main (void) {
//(pos.x * 100)%256)/256
float blue = mod(pos.z * 256, 256);
float red = mod(pos.x * 256, 256);
float green = mod(pos.y * 256, 256);

  color = vec3(red/256,green/256,blue/256);//(pos%256)/256 //* vec3(1,1,0);
  //color = vec3(1,1,0);
};