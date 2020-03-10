#ifndef renderer_h // include guard
#define renderer_h

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

class Renderer {
public:
	std::vector<GLfloat> positions;
	std::vector<GLfloat> colors;
	std::vector<GLint> indices;

	int NUM_POSITIONS = 4;	// x,y,z,w
	int NUM_COLORS = 4;		// r,g,b,a

	Renderer();
	Renderer(std::vector<GLfloat> positions, std::vector<GLfloat> colors, std::vector<GLint> indices);

	void add(glm::vec4 position, glm::vec4 color);
	int numElements();

	void load(int vao, GLuint vPosition, GLuint vColor);
	Renderer subData(float t);	
};

#endif renderer_h