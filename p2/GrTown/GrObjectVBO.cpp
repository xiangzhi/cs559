/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006
//
// CS 559 - Project 2 - Xiang Zhi Tan
//GrObjectVBO.cpp
//A direct clone of the original GrObject.h
//it still have old members due to possible legacy code that might break

#include "GrObjectVBO.h"
#include "DrawingState.H"
#include "World.h"

using std::vector;

int notTheSame = 1;
GrObjectVBO::GrObjectVBO(char* nb, int& nn, char* xn) :
ridable(0), interesting(0),
parent(0)
{
	// since its easiest to just use C sprintf to build the name
	char buf[255];
	if (!nb)
		_snprintf(buf, 255, "NoName-%d%c", nn++, 0);
	else if (xn)
		_snprintf(buf, 255, "%s-%s-%d%c", nb, xn, nn++, 0);
	else
		_snprintf(buf, 255, "%s-%d%c", nb, nn++, 0);
	name = buf;


	transform = glm::mat4(1.0f);
  localTransform = glm::mat4(1.0f);
	rideAbove = 0;
}

GrObjectVBO::GrObjectVBO(std::string _name, int& nn, char* xn) :
ridable(0), interesting(0),
parent(0){

  name = _name + xn + std::to_string(nn++);

  transform = glm::mat4(1.0f);
  localTransform = glm::mat4(1.0f);
  rideAbove = 0;

}

GrObjectVBO::~GrObjectVBO()
{
}

// the method to draw stuff
// the user have the choice of not overwritting this and uses the
// information given in the intialize to run the code
void GrObjectVBO::draw(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
  //if for some reason, something change
  if (redraw){
    //reinitialize the buffer and put stuff in.
    initialize();
    redraw = false;
  }
  //generate the PVM matrux
  glm::mat4 MVP = proj * view * model;

  //enable the vertexAttribArray
  
  //bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(
	  0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	  );

  //bind colorBuffer
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(
	  1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	  );


  //make sure the texture exist
  if (useTexture){
	  //bind before every draw
	  t->bind();
	  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	  glVertexAttribPointer(
		  2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		  2,                                // size
		  GL_FLOAT,                         // type
		  GL_FALSE,                         // normalized?
		  0,                                // stride
		  (void*)0                          // array buffer offset
		  );

	  GLuint sampleLoc = glGetUniformLocation(shaderID, "textureInput");
	  glUniform1i(sampleLoc, t->texName);
  }

  //bind normalBufferAfter
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(
	  3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size
	  GL_FLOAT,           // type
	  GL_TRUE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	  );
  glUseProgram(shaderID);
  runAttribute(proj, view, model);
  //get the location of MVP in shader
  GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
  //pass our MVP to shader
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), -ry, glm::vec3(0, 1, 0)));

  MatrixID = glGetUniformLocation(shaderID, "ry");
  glUniformMatrix3fv(MatrixID, 1, GL_FALSE, &rotationMatrix[0][0]);

  GLuint ModelID = glGetUniformLocation(shaderID, "model");
  glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);

  //passes view matrix to shader
  MatrixID = glGetUniformLocation(shaderID, "view");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0][0]);

  MatrixID = glGetUniformLocation(shaderID, "proj");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &proj[0][0]);


  //lighting calculations
  float light = calculateSunLight(drst);
  glm::vec3 sun = calculateSunDirection(drst);

  //get location of sun
  GLuint sunID = glGetUniformLocation(shaderID, "sunDirection");
  //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
  glUniform3f(sunID, sun.x, sun.y, sun.z);

  GLuint lightID = glGetUniformLocation(shaderID, "light");
  glUniform1f(lightID, light);

  lightID = glGetUniformLocation(shaderID, "lightIntensity");
  glUniform1f(lightID, light);


  if (useIndex){
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	  glDrawElements(
		  type,
		  indexNum,
		  GL_UNSIGNED_INT,
		  (void*)0
		  );
  }
  else{
	  glDrawArrays(type, 0, vertexNum);
  }

  //disable everything
  glUseProgram(0);





}

//similar as draw but for the second pass
void GrObjectVBO::drawAfter(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
  //if for some reason, something change
  if (redrawAfter){
    //reinitialize the buffer and put stuff in.
    initializeAfter();
    redrawAfter = false;
  }
  //generate the PVM matrux
  glm::mat4 MVP = proj * view * model;

  //enable the vertexAttribArray
  
  
  
  
  

  //bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferAfter);
  glVertexAttribPointer(
	  0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	  );

  //bind colorBuffer
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(
	  1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	  );


  //make sure the texture exist
  if (useTexture){
	  //bind before every draw
	  t->bind();
	  glBindBuffer(GL_ARRAY_BUFFER, textureBufferAfter);
	  glVertexAttribPointer(
		  2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		  2,                                // size
		  GL_FLOAT,                         // type
		  GL_FALSE,                         // normalized?
		  0,                                // stride
		  (void*)0                          // array buffer offset
		  );

	  GLuint sampleLoc = glGetUniformLocation(shaderIDAfter, "textureInput");
	  glUniform1i(sampleLoc, t->texName);
  }

  //bind normalBufferAfter
  glBindBuffer(GL_ARRAY_BUFFER, normalBufferAfter);
  glVertexAttribPointer(
	  3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	  3,                  // size
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  (void*)0            // array buffer offset
	  );
  glUseProgram(shaderIDAfter);
  runAttribute(proj, view, model);
  //get the location of MVP in shader
  GLuint MatrixID = glGetUniformLocation(shaderIDAfter, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);



  //passes view matrix to shader
  MatrixID = glGetUniformLocation(shaderIDAfter, "view");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0][0]);
  //passes model matrix to shader
  GLuint ModelID = glGetUniformLocation(shaderIDAfter, "model");
  glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);


  //lighting calculations
  float light = calculateSunLight(drst);
  glm::vec3 sun = calculateSunDirection(drst);

  //get location of sun
  GLuint sunID = glGetUniformLocation(shaderIDAfter, "sunDirection");
  //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
  glUniform3f(sunID, sun.x, sun.y, sun.z);

  GLuint lightID = glGetUniformLocation(shaderIDAfter, "light");
  glUniform1f(lightID, light);


  if (useIndex){
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	  glDrawElements(
		  type,
		  indexNum,
		  GL_UNSIGNED_INT,
		  (void*)0
		  );
  }
  else{
	  glDrawArrays(type, 0, vertexNumAfter);
  }

  //disable everything
  glUseProgram(0);





}

//virtual method that let child to add variables to pass to shader
void GrObjectVBO::runAttribute(glm::mat4 proj, glm::mat4 view, glm::mat4 model){
  //nothing
}
void  GrObjectVBO::runAttributeAfter(){
  //nothing
}


// for most objects, the way to use it as a camera is to take its
// transformation and invert it - potentially using "rideabove"
glm::mat4 GrObjectVBO::getCamera(){
	glm::mat4 m;
	m = glm::inverse(transform);
	m[3][1] -= rideAbove;
	return m;
}

// the user interface for the object may or may not do anything
bool GrObjectVBO::uiStep()
{
	return false;
}
int GrObjectVBO::handle(int)
{
	return 0;
}


// note - we might not really get to the time in one shot
void GrObjectVBO::simulateUntil(unsigned long t)
{
  //
}

GrObjectVBO* GrObjectVBO::add(GrObjectVBO* g, float x, float y, float z, float ry)
{
	//TODO:make sure its in the correct order
	children.push_back(g);
	g->parent = this;
	glm::mat4 m1(1.0f), m2(1.0f);
	m1 = glm::rotate(ry, glm::vec3(0, 1, 0));
	m2 = m1 * (g->transform);
	g->transform = glm::translate(m2, glm::vec3(x, y, z));
	return g;
}


GrObjectVBO* add(GrObjectVBO* g, float x, float y, float z, float ry)
{
	theVBOobjects.push_back(g);
	glm::mat4 m1(1.0f), m2(1.0f);
	m1 = glm::rotate(ry, glm::vec3(0, 1, 0));
	m2 = m1 * g->transform;
	g->transform = g->transform * glm::translate(m2, glm::vec3(x, y, z));
	return g;
}

//initialize the buffers
void GrObjectVBO::initialize(){
	//nothing yet, programs need to initialize this
}

void GrObjectVBO::initializeAfter(){
  //nothing here
}

//a method that is called before running
void GrObjectVBO::preDraw(DrawingState* drst){

}



GrObjectVBO* GrObjectVBO::clone(GrObjectVBO* temp){
  if (temp == nullptr){
    temp = new GrObjectVBO();
  }

	//copy all the information
	temp->pos = pos;
	temp->scale = scale;
	temp->vertexBuffer = vertexBuffer;
	temp->normalBuffer = normalBuffer;
	temp->textureBuffer = textureBuffer;
	temp->useTexture = useTexture;
	temp->vertexNum = vertexNum;
	temp->shaderID = shaderID;

	temp->redraw = redraw;
	temp->redrawAfter = redrawAfter;

	//secondary informations
	temp->vertexBufferAfter = vertexBufferAfter;
	temp->normalBufferAfter = normalBufferAfter;
	temp->textureBufferAfter = textureBufferAfter;
	temp->vertexNumAfter = vertexNumAfter;
	temp->shaderIDAfter = shaderIDAfter;

  temp->transform = transform;
  temp->localTransform = localTransform;


	return temp;
}


// draw a list of objects
void drawObList(vector<GrObjectVBO*>& objs, DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	for (vector<GrObjectVBO*>::iterator i = objs.begin(); i != objs.end(); ++i) {
		GrObjectVBO* g = (*i);
		//TODO::don't draw if you are camera
		g->preDraw(drst);
		glm::mat4 newModel = model * g->transform;
		g->draw(drst, proj, view, newModel * g->localTransform);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (g->children.size()) {
			drawObList(g->children, drst,proj,view, newModel);
		}
	}
}

void drawAfterObList(vector<GrObjectVBO*>& objs, DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	for (std::vector<GrObjectVBO*>::iterator i = objs.begin(); i != objs.end(); ++i) {
		GrObjectVBO* g = (*i);
		//TODO:Camera
    //apply transformtion
		g->preDraw(drst);
		glm::mat4 newModel = model * g->transform;
		g->drawAfter(drst, proj, view, newModel);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (g->children.size()) {
      //apply transformation to child
			drawAfterObList(g->children, drst, proj, view, newModel);
		}
	}
}

