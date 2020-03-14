//#include "renderer.h"

#include "common.h"
#include "geometry.h"

Geometry::Geometry() {};
Mesh::Mesh() {
	this->oldCopy = NULL;
};

Mesh::Mesh(std::vector<glm::vec3> points, std::vector<Face*> faces, Mesh * oldCopy) {
	this->points = points;
	this->faces = faces;
	this->oldCopy = oldCopy;
};

HalfEdge::HalfEdge() {};
HalfEdge::HalfEdge(int head) {
	this->head = head;
	this->next = NULL;
	this->flip = NULL;
	this->ev = NULL;
	this->face = NULL;
};

void makePair(HalfEdge*a, HalfEdge*b) {
	a->flip = b;
	b->flip = a;
}

void makeFollow(HalfEdge* a, HalfEdge* b) {
	a->next = b;
	b->prev = a;
}

Face* makeFace(HalfEdge* root) {
	bool loop = true;
	Face* myFace = new Face(root);
	int n = 0;
	HalfEdge* currEdge = root;
	while (loop) {		
		currEdge->face = myFace;
		currEdge = currEdge->next;
		n++;
		if (currEdge == root) {
			loop = false;
		}
	}
	assert(n == 4);
	return myFace;
}


void printRing(HalfEdge* root) {	
	bool loop = true;	
	HalfEdge* currEdge = root;
	int index = 0;
	while (loop) {
		if (currEdge == NULL) {
			std::cout << "[" << index << "]:	curr = NULL\n";
			break;
		}
		std::cout << "[" << index << "]:	curr = " << currEdge->head;
		std::cout << " --> next = " << currEdge->next->head;		
		if (currEdge->flip != NULL) {
			std::cout << "\n		     pair = " << currEdge->flip->head<<"\n";
		}
		std::cout << "\n";
		currEdge = currEdge->next;
		index++;
		if (currEdge == root) {
			loop = false;
		}
	}
}

// should be deprecated
HalfEdge* rotateAroundHead(HalfEdge* curr) {
	bool isTail = false;
	HalfEdge* currEdge = curr;
	int center = curr->head;
	while (!isTail) {
		currEdge = currEdge->next;
		if (currEdge->next->head == center) {
			isTail = true;
		}
	}
	return currEdge;
}

bool allHavePairs(HalfEdge* root) {
	HalfEdge* currEdge = root;

	bool loop = true;
	while (loop) {		
		if(currEdge->flip==NULL)
		{
			return false;
		}
		currEdge = currEdge->next;
		if (currEdge == root)
		{
			loop = false;
		}
	}
	return true;
}

Face::Face(HalfEdge* root) {
	this->root = root;
}


void Mesh::mergeFace(Face * newFace) {
	for (Face * currFace : faces) {
		HalfEdge * currEdge = currFace->root;
		bool loop = true;
		while (loop) { // Check each edge of an old face		
			if (currEdge->flip == NULL) { // Skip an edge if it already has a pair
				int myHead = currEdge->head;
				int myTail = currEdge->next->head;
				HalfEdge* faceEdge = newFace->root;
				bool rotateFace = true;
				while (rotateFace) {  // Rotate new face and check it matches with an existing one
					int faceHead = faceEdge->head;
					int faceTail = faceEdge->next->head;
					if (myHead == faceTail && myTail == faceHead) { // there is a match
						makePair(currEdge, faceEdge);
						rotateFace = false;
					}
					
					if (allHavePairs(newFace->root)) { // abort
						loop = false;
						rotateFace = false;
					}
					else {
						faceEdge = faceEdge->next;
						if (faceEdge == newFace->root) { // we've made a full circle
							rotateFace = false;
						}
					}
				}				
			}
			currEdge = currEdge->next;
			if (currEdge == currFace->root) {
				loop = false;
			}
		}		
	}
	faces.push_back(newFace);
}


void Mesh::buildGeometry(Geometry &geometry) {	

	geometry.positions.clear();
	geometry.elements.clear();	

	for (Face * face : faces) {
		HalfEdge * currEdge = face->root; 
				
		bool rotateFace = true;
		while (rotateFace) {			
			geometry.elements.push_back((GLuint)currEdge->head);
			currEdge = currEdge->next;
			if (currEdge == face->root)
			{
				rotateFace = false;
			}
		}
	}

	GLfloat w = 1.0;
	for (glm::vec3 p : points) {
		geometry.positions.push_back(p.x);
		geometry.positions.push_back(p.y);
		geometry.positions.push_back(p.z);
		geometry.positions.push_back(w);
	}
}


glm::vec3 getAverage(std::vector<glm::vec3> array) {
	glm::vec3 ret;
	int n = array.size();
	
	for (glm::vec3 i: array) {
		ret += i;
	}
	ret.x = ret.x / n;
	ret.y = ret.y / n;
	ret.z = ret.z / n;
	return ret;
}


// #1. Find a face point
// 
//     p - - - - p
//     .         . 
//     .    v    .    <-- average of all points (p) that create a face 
//     .         .
//     p - - - - p
//     
void Mesh::findFacePoints(std::vector<glm::vec3> &outPoints) {
	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;

		int start = currEdge->head;
		std::vector<glm::vec3> ring;
		bool loop = true;
		while (loop) {
			ring.push_back(points[currEdge->head]);
			currEdge = currEdge->next;
			if (currEdge->head == start) {
				loop = false;
			}
		}

		assert(ring.size() == 4 && "The number of face points != 4");

		glm::vec3 V = getAverage(ring);
		outPoints.push_back(V);
		face->facePoint = outPoints.size() - 1;
	}
}

// #2. We are going to find new positions for each original point p.
// For this we would need to find all adjecent faces and midpoints (from original edges)...
//         	
//          | |
//    Face   M  Face
//   __   __| |__   __
//   __ M __ P __ M __
//          | |
//    Face   M  Face
//          | |  
//
void Mesh::findVertexPoints(std::vector<glm::vec3> &outPoints) {
	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;

		// For every point on the ORIGINAL mesh 
		bool loop = true;
		while (loop) {
			if (currEdge->newHead == -1) // Check whether a point was already updated
			{
				std::vector<glm::vec3> tempFacePoints; // for face points touching an original point
				std::vector<glm::vec3> tempMidPoints; // for medians of original edges that touch a point

				int n = 0; // also count how many face are connected
				bool innerLoop = true;
				HalfEdge* currInnerEdge = currEdge;
				while (innerLoop) {
					int f = currInnerEdge->face->facePoint;
					tempFacePoints.push_back(outPoints[f]);

					int m1 = currInnerEdge->head;
					int m2 = currInnerEdge->next->head;
					std::vector<glm::vec3> tempArr = { points[m1],points[m2] };
					glm::vec3 M = getAverage(tempArr);
					tempMidPoints.push_back(M);

					// Rotate around a vertex
					currInnerEdge = currInnerEdge->prev;
					currInnerEdge = currInnerEdge->flip;
					n++;
					assert(n <= 4);

					if (currInnerEdge == currEdge) {
						innerLoop = false; // Done, we've made a circle around a vertex
					}
				}

				// Calculate averages
				glm::vec3 F = getAverage(tempFacePoints);
				glm::vec3 R = getAverage(tempMidPoints);

				// We need to use this equation, to find a new P
				// ( F + 2R + ( n-3 ) P ) / n

				glm::vec3 P = points[currEdge->head];

				R = R * glm::vec3(2, 2, 2);
				P = P * glm::vec3(n - 3, n - 3, n - 3);
				P = F + R + P;
				P = P / glm::vec3(n, n, n);

				outPoints.push_back(P);
				int p = outPoints.size() - 1;
				currEdge->newHead = p; // new original point
			}

			currEdge = currEdge->next;
			if (currEdge == face->root) {
				loop = false;
			}
		}
	}
}

// #3. Find edge points
// 
//          e  
//	            
//     e    v    e    
//	        
//          e  
//         
void Mesh::findEdgePoints(std::vector<glm::vec3>& outPoints) {
	std::vector<glm::vec3> array;

	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;

		bool loop = true;
		while (loop) {
			// first check whether this point was already created
			if (currEdge->edgePoint == -1) {

				// need 2 original endpoints
				int d1 = currEdge->head;
				int d2 = currEdge->next->head;

				// need two neighbouring face points
				int v1 = currEdge->face->facePoint;
				int v2 = currEdge->flip->face->facePoint;

				array = { points[d1], points[d2], outPoints[v1], outPoints[v2] };
				glm::vec3 E = getAverage(array);
				outPoints.push_back(E);
				int e = outPoints.size() - 1;

				currEdge->edgePoint = e;
				currEdge->flip->edgePoint = e; // propagate this point to a side face
			}
				
			currEdge = currEdge->next;
			if (currEdge == face->root)
			{
				loop = false;
			}
			
		}
	}
}


// #4. We can start making half edges
// 
//          e 
//       __| |__
//     e __ v __ e
//         | |
//          e    
//
void Mesh::mergeCorners(std::vector<glm::vec3>& outPoints) {
	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;

		bool loop = true;
		while (loop) {

			int v = currEdge->face->facePoint;
			int e = currEdge->edgePoint;
			HalfEdge* ve = new HalfEdge(v);
			HalfEdge* ev = new HalfEdge(e);
			makePair(ev, ve);
			currEdge->ev = ev;

			currEdge = currEdge->next;
			if (currEdge == face->root)
			{
				loop = false;
			}

		}
	}

	// We can also use recently updated original points to extend our half edges:
	//
	//     p __ e __ p
	//      |__| |__|
	//     e __ v __ e
	//      |__| |__|
	//     p    e    p
	//
	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;

		bool loop = true;
		while (loop) {
			HalfEdge* ve = currEdge->ev->flip;
			makeFollow(currEdge->next->ev, ve);

			// 
			//          e 
			//       __| |__
			//     e __ v __ e
			//         | |__
			//          e    p
			//

			HalfEdge* ep = new HalfEdge(currEdge->ev->head);
			makeFollow(ve, ep);

			// 
			//          e 
			//       __| |__
			//     e __ v __ e
			//         | |__|
			//          e    p
			//

			HalfEdge* pe = new HalfEdge(currEdge->next->newHead); 
			makeFollow(ep, pe);
			makeFollow(pe, currEdge->next->ev);
					   
			// If we repeat this for each Half Edge of an original Face, we would get
			//
			//       __ e __
			//      |__| |__|
			//     e __ v __ e
			//      |__| |__|
			//          e
			//

			currEdge = currEdge->next;
			if (currEdge == face->root)
			{
				loop = false;
			}
		}
	}
}

// 5. Our groups of half edges are still isolated.
// To make a mesh we need to find all the pairs from other faces:
//       __   __
//     P __ e __ P
//   |  |__| |__|  |
//     e __ v __ e
//   |  |__| |__|  |
//     P __ e __ P
//   
void Mesh::mergeSquares(std::vector<glm::vec3>& outPoints) {    
	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;

		bool loop = true;
		while (loop) {
			//     .    .    .
			//     .    .    .
			//       __ v __  
			//   |  |__| |__|  |   Current face
			//     a __ e __ b
			//       
			//        |    |      <--- first check if these are connected 
			//       __   __
			//     c __ e __ d
			//   |  |__| |__|  |   Some other face
			//       __ v __ 
			//     .    .    .
			//     .    .    .
					   
			HalfEdge* outsideEdge = currEdge->flip;

			HalfEdge* ae = currEdge->ev->prev;
			if (ae->flip == NULL) {
				HalfEdge* ec = outsideEdge->ev->flip->next; 
				makePair(ae, ec);
			}

			HalfEdge* eb = currEdge->ev->flip->next;
			if (eb->flip == NULL) {
				HalfEdge* de = outsideEdge->ev->prev;
				makePair(eb, de);
			}

			currEdge = currEdge->next;
			if (currEdge == face->root) {
				loop = false; // Done, we are at the root edge of the face
			}
		}
	}
}

// #6. We are almost done. Just need to define faces
void Mesh::populateFaces(std::vector<Face*> &outFaces) {

	for (Face* face : faces) {
		HalfEdge* currEdge = face->root;
		bool loop = true;	
		while (loop) {
			Face* subdividedFace = makeFace(currEdge->ev);
			outFaces.push_back(subdividedFace);
			currEdge = currEdge->next;

			if (currEdge == face->root) {
				loop = false; // Done, we are at the root edge of the face
			}
		}
	}
}

void Mesh::subDivide() {
	// holds final mesh data
	std::vector<glm::vec3> outPoints;
	std::vector<Face*> outFaces;

	findFacePoints(outPoints);
	findVertexPoints(outPoints);
	findEdgePoints(outPoints);
	mergeCorners(outPoints);	 	  	
	mergeSquares(outPoints);
	populateFaces(outFaces);

	oldCopy = new Mesh(points, faces, oldCopy);
	faces = outFaces;
	points = outPoints;
}

void Mesh::popSubDivision() {
	if (oldCopy!=NULL) {		
		points = oldCopy->points;
		faces = oldCopy->faces;
		oldCopy = oldCopy->oldCopy;
		//std::cout << "points size =" << points.size() << "\n";
	}	
}



bool Mesh::loadQuadObj(const char* path)
{
	// reset
	points = std::vector<glm::vec3>(); 
	faces = std::vector<Face*>(); 

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
			points.push_back(position);
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
			int A = quad[0] - 1; 
			int B = quad[1] - 1; 
			int C = quad[2] - 1;
			int D = quad[3] - 1;
			
			HalfEdge * a = new HalfEdge(A); 
			HalfEdge * b = new HalfEdge(B); 
			HalfEdge * c = new HalfEdge(C);
			HalfEdge * d = new HalfEdge(D); 
					   		
			makeFollow(a, b);
			makeFollow(b, c);
			makeFollow(c, d);
			makeFollow(d, a);

			Face* face = makeFace(a);			
			mergeFace(face);
		}

	}
	return true;
}


bool Mesh::loadTriangObj(const char* path)
{
	// reset
	points = std::vector<glm::vec3>();
	faces = std::vector<Face*>();

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
			points.push_back(position);
		}

		// Face
		else if (strcmp(lineHeader, "f") == 0) {
			GLuint pos[3];
			int matches = fscanf(file, "%d %d %d \n", &pos[0], &pos[1], &pos[2]);
			if (matches != 3) {
				printf("Sorry, I do not support this obj\n");
				return false;
			}
			// Need -1,  
			// C++ indexing starts at 0 and OBJ indexing starts at 1
			int A = pos[0] - 1;
			int B = pos[1] - 1;
			int C = pos[2] - 1;

			HalfEdge* a = new HalfEdge(A);
			HalfEdge* b = new HalfEdge(B);
			HalfEdge* c = new HalfEdge(C);

			makeFollow(a, b);
			makeFollow(b, c);
			makeFollow(c, a);

			Face* face = makeFace(a);
			mergeFace(face);
		}

	}
	return true;
}