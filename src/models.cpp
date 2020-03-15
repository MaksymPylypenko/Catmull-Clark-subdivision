#include "common.h"
#include "geometry.h"

int Mesh::addPoint(glm::vec3 point) {
	points.push_back(point);
	return points.size() - 1;
}

Face* Mesh::buildFace(std::vector<int> indecies) {
	std::vector<HalfEdge*> hedges;
	for (int index : indecies)
	{
		hedges.push_back(new HalfEdge(index));
	}
	int n = hedges.size();
	for (int i = 0; i < n - 1; i++) {
		makeFollow(hedges[i], hedges[i + 1]);
	}
	makeFollow(hedges[n - 1], hedges[0]); // close the circle, last follows first
	return populateFace(hedges[0]);
}


void Mesh::loadCrossModel() {
	// reset
	points = std::vector<glm::vec3>();
	faces = std::vector<Face*>();

	// center
	int a = addPoint(glm::vec3(0.25, -0.25, 0.25));
	int b = addPoint(glm::vec3(0.25, -0.25, -0.25));
	int c = addPoint(glm::vec3(0.25, 0.25, -0.25));
	int d = addPoint(glm::vec3(0.25, 0.25, 0.25));
	int e = addPoint(glm::vec3(-0.25, -0.25, 0.25));
	int f = addPoint(glm::vec3(-0.25, -0.25, -0.25));
	int g = addPoint(glm::vec3(-0.25, 0.25, -0.25));
	int h = addPoint(glm::vec3(-0.25, 0.25, 0.25));

	// right
	int aR = addPoint(glm::vec3(1.0, -0.25, 0.25));
	int bR = addPoint(glm::vec3(1.0, -0.25, -0.25));
	int cR = addPoint(glm::vec3(1.0, 0.25, -0.25));
	int dR = addPoint(glm::vec3(1.0, 0.25, 0.25));

	// left
	int eL = addPoint(glm::vec3(-1.0, -0.25, 0.25));
	int fL = addPoint(glm::vec3(-1.0, -0.25, -0.25));
	int gL = addPoint(glm::vec3(-1.0, 0.25, -0.25));
	int hL = addPoint(glm::vec3(-1.0, 0.25, 0.25));

	// top
	int dT = addPoint(glm::vec3(0.25, 1.0, 0.25));
	int cT = addPoint(glm::vec3(0.25, 1.0, -0.25));
	int hT = addPoint(glm::vec3(-0.25, 1.0, 0.25));
	int gT = addPoint(glm::vec3(-0.25, 1.0, -0.25));

	// bot
	int aB = addPoint(glm::vec3(0.25, -1.0, 0.25));
	int bB = addPoint(glm::vec3(0.25, -1.0, -0.25));
	int eB = addPoint(glm::vec3(-0.25, -1.0, 0.25));
	int fB = addPoint(glm::vec3(-0.25, -1.0, -0.25));
	
	// Faces visible from front
	mergeFace(buildFace(std::vector<int>{ a, d, h, e }));
	mergeFace(buildFace(std::vector<int>{ h, d, dT, hT }));
	mergeFace(buildFace(std::vector<int>{ d, a, aR, dR }));
	mergeFace(buildFace(std::vector<int>{ e, h, hL, eL }));
	mergeFace(buildFace(std::vector<int>{ a, e, eB, aB }));

	// Faces visible from right
	mergeFace(buildFace(std::vector<int>{ aR, bR, cR, dR }));
	mergeFace(buildFace(std::vector<int>{ d, c, cT, dT }));
	mergeFace(buildFace(std::vector<int>{ a, aB, bB, b }));

	// Faces visible from left
	mergeFace(buildFace(std::vector<int>{ eL, hL, gL, fL }));
	mergeFace(buildFace(std::vector<int>{ h, hT, gT, g }));
	mergeFace(buildFace(std::vector<int>{ e, f, fB, eB }));

	// Faces visible from top
	mergeFace(buildFace(std::vector<int>{ dT, cT, gT, hT }));
	mergeFace(buildFace(std::vector<int>{ c, d, dR, cR }));
	mergeFace(buildFace(std::vector<int>{ h, g, gL, hL }));

	// Faces visible from bot
	mergeFace(buildFace(std::vector<int>{ eB, fB, bB, aB }));
	mergeFace(buildFace(std::vector<int>{ f, e, eL, fL }));
	mergeFace(buildFace(std::vector<int>{ a, b, bR, aR }));

	// Faces visible from behind
	mergeFace(buildFace(std::vector<int>{ f,g,c,b }));
	mergeFace(buildFace(std::vector<int>{ g, gT, cT, c }));
	mergeFace(buildFace(std::vector<int>{ b, c, cR, bR }));
	mergeFace(buildFace(std::vector<int>{ f, b, bB, fB }));
	mergeFace(buildFace(std::vector<int>{ f, fL, gL, g }));

}


void Mesh::loadTorusModel() {
	// reset
	points = std::vector<glm::vec3>();
	faces = std::vector<Face*>();

	GLfloat depth = 0.25;

	// center
	int a = addPoint(glm::vec3(0.50, -0.50, depth));
	int b = addPoint(glm::vec3(0.50, -0.50, -depth));
	int c = addPoint(glm::vec3(0.50, 0.50, -depth));
	int d = addPoint(glm::vec3(0.50, 0.50, depth));
	int e = addPoint(glm::vec3(-0.50, -0.50, depth));
	int f = addPoint(glm::vec3(-0.50, -0.50, -depth));
	int g = addPoint(glm::vec3(-0.50, 0.50, -depth));
	int h = addPoint(glm::vec3(-0.50, 0.50, depth));

	// right
	int aR = addPoint(glm::vec3(1.0, -0.50, depth));
	int bR = addPoint(glm::vec3(1.0, -0.50, -depth));
	int cR = addPoint(glm::vec3(1.0, 0.50, -depth));
	int dR = addPoint(glm::vec3(1.0, 0.50, depth));
	   
	// left
	int eL = addPoint(glm::vec3(-1.0, -0.50, depth));
	int fL = addPoint(glm::vec3(-1.0, -0.50, -depth));
	int gL = addPoint(glm::vec3(-1.0, 0.50, -depth));
	int hL = addPoint(glm::vec3(-1.0, 0.50, depth));
	   
	// top
	int dT = addPoint(glm::vec3(0.50, 1.0, depth));
	int cT = addPoint(glm::vec3(0.50, 1.0, -depth));
	int hT = addPoint(glm::vec3(-0.50, 1.0, depth));
	int gT = addPoint(glm::vec3(-0.50, 1.0, -depth));

	// bot
	int aB = addPoint(glm::vec3(0.50, -1.0, depth));
	int bB = addPoint(glm::vec3(0.50, -1.0, -depth));
	int eB = addPoint(glm::vec3(-0.50, -1.0, depth));
	int fB = addPoint(glm::vec3(-0.50, -1.0, -depth));

	// corners
	int dRT = addPoint(glm::vec3(1.0, 1.0, depth));
	int cRT = addPoint(glm::vec3(1.0, 1.0, -depth));
	int aRB = addPoint(glm::vec3(1.0, -1.0, depth));
	int bRB = addPoint(glm::vec3(1.0, -1.0, -depth));
	int hLT = addPoint(glm::vec3(-1.0, 1.0, depth));
	int gLT = addPoint(glm::vec3(-1.0, 1.0, -depth));
	int eLB = addPoint(glm::vec3(-1.0, -1.0, depth));
	int fLB = addPoint(glm::vec3(-1.0, -1.0, -depth));

	// Faces visible from front
	mergeFace(buildFace(std::vector<int>{ h, d, dT, hT }));
	mergeFace(buildFace(std::vector<int>{ d, a, aR, dR }));
	mergeFace(buildFace(std::vector<int>{ e, h, hL, eL }));
	mergeFace(buildFace(std::vector<int>{ a, e, eB, aB }));

	mergeFace(buildFace(std::vector<int>{ h, hT, hLT, hL }));
	mergeFace(buildFace(std::vector<int>{ e, eL, eLB, eB }));
	mergeFace(buildFace(std::vector<int>{ d, dR, dRT, dT }));
	mergeFace(buildFace(std::vector<int>{ a, aB, aRB, aR }));

	// Faces visible from right
	mergeFace(buildFace(std::vector<int>{ aR, bR, cR, dR }));
	mergeFace(buildFace(std::vector<int>{ dR, cR, cRT, dRT }));
	mergeFace(buildFace(std::vector<int>{ aR, aRB, bRB, bR }));
	mergeFace(buildFace(std::vector<int>{ e,f,g,h }));

	// Faces visible from left
	mergeFace(buildFace(std::vector<int>{ eL, hL, gL, fL }));
	mergeFace(buildFace(std::vector<int>{ hL, hLT, gLT, gL }));
	mergeFace(buildFace(std::vector<int>{ eL, fL, fLB, eLB }));
	mergeFace(buildFace(std::vector<int>{ a,d,c,b }));

	// Faces visible from top
	mergeFace(buildFace(std::vector<int>{ dT, cT, gT, hT }));
	mergeFace(buildFace(std::vector<int>{ hT, gT, gLT, hLT }));
	mergeFace(buildFace(std::vector<int>{ dT, dRT,cRT, cT }));
	mergeFace(buildFace(std::vector<int>{ a,b,f,e })); 

	// Faces visible from bot
	mergeFace(buildFace(std::vector<int>{ eB, fB, bB, aB }));
	mergeFace(buildFace(std::vector<int>{ aB, bB, bRB, aRB }));
	mergeFace(buildFace(std::vector<int>{ eB, eLB, fLB, fB }));
	mergeFace(buildFace(std::vector<int>{ d,h,g,c})); 

	// Faces visible from behind
	mergeFace(buildFace(std::vector<int>{ g, gT, cT, c }));
	mergeFace(buildFace(std::vector<int>{ b, c, cR, bR }));
	mergeFace(buildFace(std::vector<int>{ f, b, bB, fB }));
	mergeFace(buildFace(std::vector<int>{ f, fL, gL, g }));

	mergeFace(buildFace(std::vector<int>{ gLT, gT, g, gL }));
	mergeFace(buildFace(std::vector<int>{ fLB, fL, f, fB }));
	mergeFace(buildFace(std::vector<int>{ c, cT, cRT, cR }));
	mergeFace(buildFace(std::vector<int>{ b, bR, bRB, bB }));
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
			int a = quad[0] - 1;
			int b = quad[1] - 1;
			int c = quad[2] - 1;
			int d = quad[3] - 1;
			std::vector<int> array = { a,b,c,d };
			Face* face = buildFace(array);
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
			int a = pos[0] - 1;
			int b = pos[1] - 1;
			int c = pos[2] - 1;
			std::vector<int> array = { a,b,c };
			Face* face = buildFace(array);
			mergeFace(face);
		}

	}
	return true;
}