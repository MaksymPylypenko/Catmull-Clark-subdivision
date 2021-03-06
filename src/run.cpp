#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "geometry.h"

#include <iostream>
#include <vector>

const char* WINDOW_TITLE = "Subdivision";
const double FRAME_RATE_MS = 1000.0 / 60.0;


// Uniforms
GLuint ModelView;
GLuint Projection;


///----------------------------------------------------------------------------
/// Building initial geometry

enum Models { Cube, Cross, Torus, Tetrahedron, Dodecahedron, Gourd};

int currDepth = 0;
int currModel = Cube;
float viewerPosition = 5.0;

Geometry geometry;
Mesh mesh;
int lineLoop = 4; 

void setupGeometry() {
	viewerPosition = 5.0;

	if (currModel == Cube) {
		lineLoop = 4;
		mesh.loadQuadObj("obj/cube.obj");
	}
	else if (currModel == Tetrahedron){
		lineLoop = 3;
		mesh.loadTriangObj("obj/tetrahedron.obj");
	}
	else if (currModel == Dodecahedron) {
		lineLoop = 3;
		mesh.loadTriangObj("obj/dodecahedron.obj");
	}
	else if (currModel == Gourd) {
		lineLoop = 3;
		mesh.loadTriangObj("obj/gourd.obj");
	}
	else if (currModel == Cross) {
		lineLoop = 4;
		mesh.loadCrossModel();
	}
	else if (currModel == Torus) {
		lineLoop = 4;
		mesh.loadTorusModel();
	}
	for (int i = 0; i < currDepth; i++)
	{
		lineLoop = 4;
		mesh.subDivide();
		viewerPosition -= 0.5;
	}
	mesh.buildGeometry(geometry);
	
}


///----------------------------------------------------------------------------
/// Loads current geometry to the GPU

void bindGeometry() {

	glBufferData(
		GL_ARRAY_BUFFER,
		geometry.positions.size() * sizeof(GLfloat),
		geometry.positions.data(),
		GL_STATIC_DRAW
	);	   

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		geometry.elements.size() * sizeof(GLuint),
		geometry.elements.data(),
		GL_STATIC_DRAW
	);
	std::cout << "\n-------------------\n";
	std::cout << "Positions = " << mesh.points.size() << "\n";
	std::cout << "Faces = " << mesh.faces.size() << "\n";
	std::cout << "Elements = " << geometry.elements.size() << "\n";
}


///----------------------------------------------------------------------------
/// OpenGL initialization

void init() {
	
	// Initialize VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Initialize VBO for Vertex data		
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Initialize VBO for Elements (indices)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);

	// Setup and load geomerty to GPU		
	setupGeometry();
	bindGeometry();

	// Load shaders and use the resulting shader program
	GLuint shader = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(shader);

	GLuint vPosition = glGetAttribLocation(shader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	Projection = glGetUniformLocation(shader, "Projection");
	ModelView = glGetUniformLocation(shader, "ModelView");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


///----------------------------------------------------------------------------
/// Called every frame 

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
	glm::vec3 viewer_pos(0.0, 0.0, viewerPosition);


	//  Generate model-view matrices
	glm::mat4 model_view;
	{
		glm::mat4 trans, rot;
		trans = glm::translate(trans, -viewer_pos);

		rot = glm::rotate(rot, glm::radians(Theta[Xaxis]), glm::vec3(1, 0, 0));
		rot = glm::rotate(rot, glm::radians(Theta[Yaxis]), glm::vec3(0, 1, 0));
		rot = glm::rotate(rot, glm::radians(Theta[Zaxis]), glm::vec3(0, 0, 1));
		model_view = trans * rot;
	}

	glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
	

	for (int i = 0; i < geometry.elements.size(); i += lineLoop) {
		glDrawElements(GL_LINE_LOOP, lineLoop, GL_UNSIGNED_INT, (void*)(i * sizeof(GLuint)));
	}

	//glDrawElements(GL_QUADS, geometry.elements.size(), GL_UNSIGNED_INT, 0);
	glutSwapBuffers();
}


///----------------------------------------------------------------------------
/// Interaction

bool pause = false;
void keyboard(unsigned char key, int x, int y)
{
		
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case ' ':  // hold
		pause = !pause;
		break;
	case 'w': case 'W':
		currDepth++;
		setupGeometry();
		bindGeometry();
		break;
	case 's': case 'S':		
		if (currDepth >= 1) {
			currDepth--;
			setupGeometry();
			bindGeometry();
		}		
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		currModel = (int)key - 49;
		currDepth = 0;
		setupGeometry();
		bindGeometry();
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
	if (!pause) {
		Theta[Axis] += 0.5;

		if (Theta[Axis] > 360.0) {
			Theta[Axis] -= 360.0;
		}
	}	
}


///----------------------------------------------------------------------------
/// Projection

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	GLfloat aspect = GLfloat(width) / height;
	glm::mat4  projection = glm::perspective(glm::radians(45.0f), aspect, 0.3f, 20.0f);
	glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projection));
}