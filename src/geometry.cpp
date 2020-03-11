<<<<<<< HEAD
//#include "renderer.h"

#include "common.h"
#include "geometry.h"


/// Vertex --> goes to GPU buffer eventually 
//Vertex::Vertex(glm::vec4 position, glm::vec4 color)
//{
//	this->position = position;
//	this->color = color;
//}



HalfEdge::HalfEdge(int head, int tail) {
	this->head = head;
	this->tail = tail;
	this->next = NULL;
	this->flip = NULL;
	this->root = NULL;
	this->isComplete = false;
};

Mesh::Mesh() {};

void Mesh::mergeFace(HalfEdge * newF) {
	for (HalfEdge currFace : faces) {
		HalfEdge * currEdge = &currFace;
		bool loopFaces = true;
		while (loopFaces) {
			{
				if (currEdge->flip != NULL) { // skip if already has a match
					int a1 = currEdge->head;
					int b1 = currEdge->tail;
					bool rotateNewFace = true;
					while (rotateNewFace) {
						int a2 = newF->head;
						int b2 = newF->tail;
						if (a1 == b2 && b1 == a2) {
							// we found a match
							newF->flip = currEdge;
							currEdge->flip = newF;
							// can abort here ..., 
							// but newF should be back to initial position
							// @TODO
						}
						newF = newF->next;
						if (newF == newF->root)
						{
							rotateNewFace = false;
						}
					}
				}
			}
			currEdge = currEdge->next;
			if (currEdge == currEdge->root) {
				loopFaces = false;
			}
		}
	}
}

bool loadQuadObj(const char* path, vector<GLfloat>& positions,
	vector<GLuint>& elements, Mesh &mesh)
{

	std::vector< glm::vec3 > temp_positions;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.


		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 position;
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			temp_positions.push_back(position);
			GLfloat w = 1.0;
			positions.push_back(position.x);
			positions.push_back(position.y);
			positions.push_back(position.z);
			positions.push_back(w);
		}

		// Face
		else if (strcmp(lineHeader, "f") == 0) {
			GLuint quad[4];
			int matches = fscanf(file, "%d %d %d %d \n", &quad[0], &quad[1], &quad[2], &quad[3]);
			if (matches != 4) {
				printf("Sorry, I do not support this obj\n");
				return false;
			}
			// Need -1,  
			// C++ indexing starts at 0 and OBJ indexing starts at 1
			int a = quad[0] - 1; elements.push_back(a);
			int b = quad[1] - 1; elements.push_back(b);
			int c = quad[2] - 1; elements.push_back(c);
			int d = quad[3] - 1; elements.push_back(d);
			
			HalfEdge ab = HalfEdge(a, b); 
			HalfEdge bc = HalfEdge(b, c); 
			HalfEdge cd = HalfEdge(c, d);
			HalfEdge da = HalfEdge(d, a); 

			ab.root = &ab;
			bc.root = &ab;
			cd.root = &ab;
			da.root = &ab;

			ab.next = &bc;
			bc.next = &cd;
			cd.next = &da;
			da.next = &ab;

			mesh.mergeFace(&ab);
		}

	}
	return true;
}
=======
#include "renderer.h"

#include "common.h"
#include "geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

/// Vertex --> goes to GPU buffer eventually 
//Vertex::Vertex(glm::vec4 position, glm::vec4 color)
//{
//	this->position = position;
//	this->color = color;
//}
>>>>>>> 2c5509930b418b34792faf59af91255a2dce8519
