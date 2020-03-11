#ifndef geometry_h // include guard
#define geometry_h

#include "common.h"
#include "geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <fstream>      // std::ifstream
#include <vector>		// std::vector
#include <sstream>      // std::istringstream
#include <string>       // std::string

using namespace std;

class HalfEdge {
public:
	int head;
	int tail;
	HalfEdge* next; // next on the circle (counter clock-wise)
	HalfEdge* flip; // flip to a different circle	
	HalfEdge* root; // start of a circle
	bool isComplete; // true when non of the pointers is NULL
	HalfEdge::HalfEdge(int head, int tail);
};

class Mesh {
public:
	std::vector<HalfEdge> faces;
	void mergeFace(HalfEdge * f);
	Mesh();
};



/// ---------------------------------------------------------------------------------
/// Load geometry from a .OBJ file --> to C++ vectors
/// Inspired by these tutorials
/// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
/// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

bool loadQuadObj(const char* path, vector<GLfloat>& positions,
	vector<GLuint>& elements, Mesh& mesh);


#endif geometry_h