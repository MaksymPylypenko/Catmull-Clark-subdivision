//#include "renderer.h"

#include "common.h"
#include "geometry.h"


HalfEdge::HalfEdge(int head, int tail) {
	this->head = head;
	this->tail = tail;
	this->next = NULL;
	this->flip = NULL;
};

Face::Face(HalfEdge* root) {
	this->root = root;
}

Mesh::Mesh() {};


void Mesh::mergeFace(Face * newFace) {
	for (Face * currFace : faces) {
		HalfEdge * currEdge = currFace->root;
		bool loopFaces = true;
		while (loopFaces) { // find faces that are friends (pairs) <3
			{
				if (currEdge->flip != NULL) { // skip if already has a friend
					int a1 = currEdge->head;
					int b1 = currEdge->tail;
					HalfEdge* currEdge2 = newFace->root;
					bool rotateNewFace = true;
					while (rotateNewFace) {
						int a2 = currEdge2->head;
						int b2 = currEdge2->tail;
						if (a1 == b2 && b1 == a2) {
							// we found a match
							currEdge2->flip = currEdge;
							currEdge->flip = currEdge2;
							// can abort here ..., 
							// unless we have 3 faces sharing the same edge, which is not the case
							rotateNewFace = false;
						}
						currEdge2 = currEdge2->next;
						// @TODO
						// add logic to stop if a new Face has all edges merged
						if (currEdge2 == newFace->root) // we've made a full circle
						{
							rotateNewFace = false;
						}
					}
				}
			}
			currEdge = currEdge->next;
			if (currEdge == currFace->root) {
				loopFaces = false;
			}
		}			
	}
	faces.push_back(newFace);
}

void Mesh::updateElements() {
	std::cout << "Ready to get elements\n";
	std::cout << "Faces = " << faces.size() << "\n";
	elements = std::vector<GLuint>(); // remove old
	for (Face * face : faces) {
		HalfEdge * currEdge = face->root;
		std::cout << "Face vertex index = " << face->root->head << "\n";
		std::cout << "Face root address = " << face->root << "\n";		
		std::cout << "Circle\n[" << "\n";	
		std::cout << "	curr = " << currEdge << "\n";
		std::cout << "	curr -> next = " << currEdge->next << "\n";
		std::cout << "	curr -> next -> next = " << currEdge->next->next << "\n";
		std::cout << "	curr -> next -> next -> next = " << currEdge->next->next->next << "\n";
		std::cout << "	curr -> next -> next -> next -> next = " << currEdge->next->next->next->next << "\n";
		std::cout << "]" << "\n";

		
		bool rotateFace = true;
		while (rotateFace) {			
			elements.push_back(currEdge->head);
			currEdge = currEdge->next;
			if (currEdge == face->root)
			{
				rotateFace = false;
			}
		}
	}
}


void Mesh::subDivide() {
	for (Face * face : faces) {
		HalfEdge * currEdge = face->root;

		std::vector<int> facePointElements;

		bool rotateFace = true;
		while (rotateFace) {		
			facePointElements.push_back(currEdge->head);
			currEdge = currEdge->next;
			if (currEdge == face->root)
			{
				rotateFace = false;
			}
		}

		glm::vec3 facePoint;
		for (int i : facePointElements) {

		}


		
	}
}

bool Mesh::loadQuadObj(const char* path)
{
	// reset
	positions = std::vector<GLfloat>(); 
	elements = std::vector<GLuint>(); 
	faces = std::vector<Face*>(); 

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
			
			HalfEdge * ab = new HalfEdge(a, b); 
			HalfEdge * bc = new HalfEdge(b, c); 
			HalfEdge * cd = new HalfEdge(c, d);
			HalfEdge * da = new HalfEdge(d, a); 
					   			
			ab->next = bc;
			bc->next = cd;
			cd->next = da;
			da->next = ab;

			Face * face = new Face(ab);
			
			mergeFace(face);
		}

	}
	return true;
}