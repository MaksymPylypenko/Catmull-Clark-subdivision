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

class Face;

class HalfEdge {
public:
	HalfEdge::HalfEdge();
	HalfEdge::HalfEdge(int head);

	int head;
	HalfEdge* prev;
	HalfEdge* next; // next on the circle (counter clock-wise)
	HalfEdge* flip; // flip to a different circle	

	// For subdivision
	Face* face; 
	HalfEdge* ev;  // edge from curr to face point
	int edgePoint = -1;
	int newHead = -1;
};

// Utility functions
void makePair(HalfEdge* a, HalfEdge* b);
void makeFollow(HalfEdge* a, HalfEdge* b);
HalfEdge* rotateAroundHead(HalfEdge* curr);
Face* populateFace(HalfEdge* root);
bool allHavePairs(HalfEdge* root);

//Debug
void printRing(HalfEdge* root);

class Face {
public:
	Face::Face(HalfEdge* root);
	HalfEdge* root; // connects face to the rest of the geometry

	// For subdivision
	int facePoint = -1; // index in a temp array
	int barycenter = -1; //  index in a temp array
};

class Mesh {
public:
	Mesh::Mesh();
	Mesh::Mesh(std::vector<glm::vec3> points, std::vector<Face*> faces, Mesh * oldCopy);
	std::vector<glm::vec3> points;
	std::vector<Face *> faces;

	// For building geometry
	int addPoint(glm::vec3 point); // returns id;
	int addPoint(glm::vec3 point, std::vector<glm::vec3> &array); // returns id;
	void buildGeometry(Geometry &geometry);
	Face* buildFace(std::vector<glm::vec3> facePoints);
	Face* buildFace(std::vector<int> indecies);
	void mergeFace(Face * f);	

	// For subdivision
	Mesh * oldCopy;
	void subDivide();
	void popSubDivision();

	void findFacePoints(std::vector<glm::vec3>& outPoints);
	void findVertexPoints(std::vector<glm::vec3>& outPoints);
	void findEdgePoints(std::vector<glm::vec3>& outPoints);
	void mergeCorners(std::vector<glm::vec3>& outPoints);
	void mergeSquares(std::vector<glm::vec3>& outPoints);
	void populateFaces(std::vector<Face*>& outFaces);

	// Load geometry from a .OBJ file --> to C++ vectors
	// Inspired by these tutorials
	// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

	bool loadQuadObj(const char* path);
	bool loadTriangObj(const char* path);

	// Default models
	void Mesh::loadCrossModel();
	void Mesh::loadTorusModel();
};


glm::vec3 getAverage(std::vector<glm::vec3> points);

#endif geometry_h