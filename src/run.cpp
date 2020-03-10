#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "renderer.h"
#include "geometry.h"

#include <iostream>
#include <vector>

const char* WINDOW_TITLE = "Subdivision";
const double FRAME_RATE_MS = 1000.0 / 60.0;



///----------------------------------------------------------------------------
/// Building initial geometry

class Vertex {
	glm::vec4 position;
	glm::vec4 color;
	HalfEdge * halfEdge;

	Vertex(glm::vec4 position, glm::vec4 color)
	{
		this->position = position;
		this->color = color;
		this->halfEdge = NULL;
	}
};

class HalfEdge {
	int head;
	int tail;
	HalfEdge * next; // next on the circle (counter clock-wise)
	HalfEdge * flip; // flip to a different circle
	HalfEdge * face; // root of the circle

	HalfEdge(int head, int tail) {
		this->head = head;
		this->tail = tail;
		this->next = NULL;
		this->flip = NULL;
		this->face = NULL;
	}
};


std::vector<Vertex> vertecies;


void setupGeometry() {	
	// Make a cube
	glm::vec4 color(1.0, 0.0, 0.0, 1.0);
	glm::vec4 position(-0.5, -0.5, 0.5, 1.0);	vertecies.push_back(Vertex(position, color)); // 0
	glm::vec4 position(-0.5, 0.5, 0.5, 1.0);	vertecies.push_back(Vertex(position, color)); // 1
	glm::vec4 position(0.5, 0.5, 0.5, 1.0); 	vertecies.push_back(Vertex(position, color)); // 2
	glm::vec4 position(-0.5, -0.5, -0.5, 1.0);	vertecies.push_back(Vertex(position, color)); // 3
	glm::vec4 position(-0.5, 0.5, -0.5, 1.0);	vertecies.push_back(Vertex(position, color)); // 4
	glm::vec4 position(0.5, 0.5, -0.5, 1.0);	vertecies.push_back(Vertex(position, color)); // 5
	glm::vec4 position(0.5, -0.5, -0.5, 1.0);	vertecies.push_back(Vertex(position, color)); // 6
	
	
}


///----------------------------------------------------------------------------
/// Called whenever geometry is changed

void bindGeometry() {
	// Need to load mesh to --> GPU
}


///----------------------------------------------------------------------------
/// OpenGL initialization

GLuint shader;
GLuint vPosition;
GLuint vColor;
GLuint VAO;

void init() {
	setupGeometry();

	// Load shaders and use the resulting shader program
	shader = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(shader);

	vPosition = glGetAttribLocation(shader, "vPosition");
	vColor = glGetAttribLocation(shader, "vColor");
	   
	// Load geomerty to GPU			
	bindGeometry();
		   
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}



///----------------------------------------------------------------------------
/// Called every frame 

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLuint ModelView;


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		const glm::vec3 viewer_pos(0.0, 0.0, 2.0);
		glm::mat4 trans, rot, model_view;
		trans = glm::translate(trans, -viewer_pos);
		rot = glm::rotate(rot, glm::radians(Theta[Xaxis]), glm::vec3(1, 0, 0));
		rot = glm::rotate(rot, glm::radians(Theta[Yaxis]), glm::vec3(0, 1, 0));
		rot = glm::rotate(rot, glm::radians(Theta[Zaxis]), glm::vec3(0, 0, 1));
		model_view = trans * rot;
		glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
	}

	// draw arrays?

	glutSwapBuffers();
}


///----------------------------------------------------------------------------
/// Interaction


int mode;
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case ' ':  // hold
		mode++;
		if (mode > 2) {
			mode = 0;
		}
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}

void update(void)
{
	Theta[Axis] += 0.5;

	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}
}


///----------------------------------------------------------------------------
/// Projection

GLuint Projection;

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat aspect = GLfloat(width) / height;
	glm::mat4  projection = glm::perspective(glm::radians(45.0f), aspect, 0.5f, 3.0f);

	glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projection));
}