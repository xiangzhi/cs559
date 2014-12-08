#include "Particles.h"
#include <algorithm>
#include "Utilities\ShaderTools.H"


// CPU representation of a particle
struct Particle{
  glm::vec3 pos, speed;
  unsigned char r, g, b, a; // Color
  float size, angle, weight;
  float life; // Remaining life of the particle. if <0 : dead and unused.
  float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

  bool operator<(const Particle& that) const {
    // Sort in reverse order : far particles drawn first.
    return this->cameradistance > that.cameradistance;
  }
};

const int MaxParticles = 100000;
static Particle ParticlesContainer[MaxParticles];
static int LastUsedParticle = 0;

// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle(){

  for (int i = LastUsedParticle; i<MaxParticles; i++){
    if (ParticlesContainer[i].life < 0){
      LastUsedParticle = i;
      return i;
    }
  }

  for (int i = 0; i<LastUsedParticle; i++){
    if (ParticlesContainer[i].life < 0){
      LastUsedParticle = i;
      return i;
    }
  }

  return 0; // All particles are taken, override the first one
}

void SortParticles(){
  std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}




void drawParticles(glm::vec3 camPos){

  static bool once = true;
  static GLuint billboard_vertex_buffer; 
  static GLuint particles_position_buffer;
  static GLuint particles_color_buffer;
  static GLuint shaderID;
  if (once){
    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    static const GLfloat g_vertex_buffer_data[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
    };
    
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
   
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    char* err;
    shaderID = loadShader("simpleVertex.glsl", "simpleFragment.glsl", err);
    once = false;
  }



  // Update the buffers that OpenGL uses for rendering.
  // There are much more sophisticated means to stream data from the CPU to the GPU, 
  // but this is outside the scope of this tutorial.
  // http://www.opengl.org/wiki/Buffer_Object_Streaming

  static double delta = 0.1f;


  static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
  static GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];

  // Generate 10 new particule each millisecond,
  // but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
  // newparticles will be huge and the next frame even longer.
  int newparticles = (int)(delta*10000.0);
  if (newparticles > (int)(0.016f*10000.0))
    newparticles = (int)(0.016f*10000.0);

  for (int i = 0; i<newparticles; i++){
    int particleIndex = FindUnusedParticle();
    ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
    ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f);

    float spread = 1.5f;
    glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
    // Very bad way to generate a random direction;
    // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
    // combined with some user-controlled parameters (main direction, spread, etc)
    glm::vec3 randomdir = glm::vec3(
      (rand() % 2000 - 1000.0f) / 1000.0f,
      (rand() % 2000 - 1000.0f) / 1000.0f,
      (rand() % 2000 - 1000.0f) / 1000.0f
      );

    ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


    // Very bad way to generate a random color
    ParticlesContainer[particleIndex].r = rand() % 256;
    ParticlesContainer[particleIndex].g = rand() % 256;
    ParticlesContainer[particleIndex].b = rand() % 256;
    ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

    ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

  }






  // Simulate all particles
  int ParticlesCount = 0;
  for (int i = 0; i<MaxParticles; i++){

    Particle& p = ParticlesContainer[i]; // shortcut

    if (p.life > 0.0f){

      // Decrease life
      p.life -= delta;
      if (p.life > 0.0f){

        // Simulate simple physics : gravity only, no collisions
        p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
        p.pos += p.speed * (float)delta;
        p.cameradistance = glm::length(p.pos - camPos);
        //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

        // Fill the GPU buffer
        g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
        g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
        g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

        g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

        g_particule_color_data[4 * ParticlesCount + 0] = p.r;
        g_particule_color_data[4 * ParticlesCount + 1] = p.g;
        g_particule_color_data[4 * ParticlesCount + 2] = p.b;
        g_particule_color_data[4 * ParticlesCount + 3] = p.a;

      }
      else{
        // Particles that just died will be put at the end of the buffer in SortParticles();
        p.cameradistance = -1.0f;
      }

      ParticlesCount++;
    }
  }


  //glUseProgram(shaderID);

  glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
  glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
  glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat)* 4, g_particule_position_size_data);

  glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
  glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
  glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte)* 4, g_particule_color_data);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
  glVertexAttribPointer(
    0, // attribute. No particular reason for 0, but must match the layout in the shader.
    3, // size
    GL_FLOAT, // type
    GL_FALSE, // normalized?
    0, // stride
    (void*)0 // array buffer offset
    );

  // 2nd attribute buffer : positions of particles' centers
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
  glVertexAttribPointer(
    1, // attribute. No particular reason for 1, but must match the layout in the shader.
    4, // size : x + y + z + size => 4
    GL_FLOAT, // type
    GL_FALSE, // normalized?
    0, // stride
    (void*)0 // array buffer offset
    );

  // 3rd attribute buffer : particles' colors
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
  glVertexAttribPointer(
    2, // attribute. No particular reason for 1, but must match the layout in the shader.
    4, // size : r + g + b + a => 4
    GL_UNSIGNED_BYTE, // type
    GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
    0, // stride
    (void*)0 // array buffer offset
    );
  
  // These functions are specific to glDrawArrays*Instanced*.
  // The first parameter is the attribute buffer we're talking about.
  // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
  // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
  glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
  glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
  glVertexAttribDivisor(2, 1); // color : one per quad -> 1

  // Draw the particules !
  // This draws many times a small triangle_strip (which looks like a quad).
  // This is equivalent to :
  // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
  // but faster.
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);



}