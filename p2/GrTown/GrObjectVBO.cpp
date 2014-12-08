/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006
//

#include "GrObjectVBO.h"
#include "Behavior.H"
#include "GrWorld.H"
#include "DrawingState.H"

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

// null functions - not abstract since not all objects will override
// them
void GrObjectVBO::draw(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	//nothing
}
void GrObjectVBO::drawAfter(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
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
// so let it take up to 10 tries
void GrObjectVBO::simulateUntil(unsigned long t)
{
	for (vector<Behavior*>::iterator i = behaviors.begin(); i != behaviors.end(); ++i) {
		Behavior *b = *i;
		int ct = 0;
		while ((b->lastV < t) && (ct<10)) {
			b->simulateUntil(t);
			ct++;
		}
		/* if (ct>1)
		printf("needed %d simulation substeps\n",ct); */
		// this is a common bug - you'll get caught here if you don't update lastV
		if (ct>8)
			printf("Warning! stuck behavior!\n");
	}
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


void GrObjectVBO::initialize(){
	//nothing yet, programs need to initialize this
}

void GrObjectVBO::preDraw(){

};


void GrObjectVBO::realDraw(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model){
	
	//if for some reason, something change
	if (redraw){
		//reinitialize the buffer and put stuff in.
		initialize();
		redraw = false;
	}

	//generate the PVM matrux
	glm::mat4 MVP = proj * view * model;

	//enable the vertexAttribArray
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

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

	//bind normalBuffer
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glUseProgram(shaderID);
	//get the location of MVP in shader
	GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
	//pass our MVP to shader
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  //lighting calculations
  float light = 0;
  //day time
  glm::vec3 sun(0, 0, 0);
  if (drst->timeOfDay > 6 && drst->timeOfDay < 18){
    light = 6 - abs(12 - drst->timeOfDay);
    float angle = (drst->timeOfDay - 6) / 3 * 45;
    sun = glm::rotateZ(glm::vec3(1, 0, 0), angle);
  }



	//get location of sun
	GLuint sunID = glGetUniformLocation(shaderID, "sunDirection");
	//glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
	glUniform3f(sunID, sun.x, sun.y, sun.z);

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
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
}

// draw a list of objects
void drawObList(vector<GrObjectVBO*>& objs, DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	for (vector<GrObjectVBO*>::iterator i = objs.begin(); i != objs.end(); ++i) {
		GrObjectVBO* g = (*i);
		//TODO::don't draw if you are camera
    g->preDraw();
    glm::mat4 newModel = model * g->transform;
		g->realDraw(drst,proj,view, newModel * g->localTransform);
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
		glm::mat4 newModel = model * g->transform;
		g->drawAfter(drst, proj, view, newModel);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (g->children.size()) {
      //apply transformation to child
			drawAfterObList(g->children, drst, proj, view, newModel);
		}
	}
}


// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/GrObjectVBO.cpp,v 1.4 2008/11/14 19:53:30 gleicher Exp $
