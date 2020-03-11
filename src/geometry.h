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


class Geometry {
public:
	Geometry::Geometry();
	std::vector<GLfloat> positions;
	std::vector<GLuint> elements;
};


class HalfEdge {
public:
	HalfEdge::HalfEdge(int head, int tail);

	int head;
	int tail;
	HalfEdge* next; // next on the circle (counter clock-wise)
	HalfEdge* flip; // flip to a different circle	

	// might need to add a pointer to the face in the future...
};

class Face {
public:
	Face::Face(HalfEdge* root);
	HalfEdge* root; // connects face to the rest of the geometry
	std::vector<HalfEdge*> subDivided;	
};

class Mesh {
public:
	Mesh::Mesh();
	std::vector<glm::vec3> points;
	std::vector<Face *> faces;

	void buildGeometry(Geometry &geometry);
	void mergeFace(Face * f);
	void subDivide();

	// Load geometry from a .OBJ file --> to C++ vectors
	// Inspired by these tutorials
	// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

	bool loadQuadObj(const char* path);
};

#endif geometry_h