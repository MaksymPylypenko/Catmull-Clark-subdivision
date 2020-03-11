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
	this->root = NULL; // Not really required.., just for convenience 
	this->isComplete = false;
};

Mesh::Mesh() {};

void Mesh::mergeFace(HalfEdge * newF) {
	std::cout << "\nMerging... "<< "\n";
	for (HalfEdge * currFace : faces) {
		HalfEdge * currEdge = currFace;
		std::cout << "Root of a face = " << currEdge->head << "\n";
		bool loopFaces = true;
		while (loopFaces) { // find faces that are friends (pairs) <3
			{
				if (currEdge->flip != NULL) { // skip if already has a friend
					int a1 = currEdge->head;
					int b1 = currEdge->tail;
					HalfEdge* currEdge2 = newF;
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
						if (currEdge2 == newF) // we've made a full circle
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
	faces.push_back(newF);
}

std::vector<GLuint> Mesh::getElements() {
	std::cout << "Ready to get elements\n";
	std::cout << "Faces = " << faces.size() << "\n";
	std::vector<GLuint> elements;
	for (HalfEdge * face : faces) {
		HalfEdge * currEdge = face;
		std::cout << "Face vertex index = " << face->head << "\n";
		std::cout << "Face address = " << face << "\n";		
		std::cout << "Circle\n[" << "\n";	
		std::cout << "	root = " << currEdge->root << "\n";
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
			if (currEdge == face)
			{
				rotateFace = false;
			}
		}
	}
	return elements;
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
			
			HalfEdge * ab = new HalfEdge(a, b); 
			HalfEdge * bc = new HalfEdge(b, c); 
			HalfEdge * cd = new HalfEdge(c, d);
			HalfEdge * da = new HalfEdge(d, a); 

			ab->root = ab;
			bc->root = ab;
			cd->root = ab;
			da->root = ab;
			
			da->next = ab;
			ab->next = bc;
			bc->next = cd;
			cd->next = da;
			
			//std::cout << "* ab = " << ab << "\n";
			//std::cout << "* bc = " << bc << "\n";
			//std::cout << "* cd = " << cd << "\n";
			//std::cout << "* da = " << da << "\n";
			//std::cout << "* da->next = " << da->next << "\n";

			mesh.mergeFace(ab);
		}

	}
	return true;
}