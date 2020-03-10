#include "renderer.h"

// Holds data that will be moved to GPU
Renderer:: Renderer() 
{
	positions = std::vector<GLfloat>();
	colors = std::vector<GLfloat>();
	indices = std::vector<GLint>();
}

Renderer:: Renderer(std::vector<GLfloat> positions, std::vector<GLfloat> colors, std::vector<GLint> indices)
{
	this->positions = positions;
	this->colors = colors;
	this->indices = indices;
}

void Renderer:: add(glm::vec4 position, glm::vec4 color) 
{
	positions.push_back(position.x);
	positions.push_back(position.y);
	positions.push_back(position.y);
	positions.push_back(position.w);
	colors.push_back(color.r);
	colors.push_back(color.g);
	colors.push_back(color.b);
	colors.push_back(color.a);
	indices.push_back(indices.size());
}

int Renderer:: numElements() {
	return indices.size();
}

void Renderer:: load(int vao, GLuint vPosition, GLuint vColor) {
	std::size_t positionsSize = sizeof(GLfloat) * numElements() * NUM_POSITIONS;
	std::size_t colorsSize = sizeof(GLfloat) * numElements() * NUM_COLORS;
	std::size_t totalSize = positionsSize + colorsSize;
	std::size_t indicesSize = sizeof(GLuint) * numElements();

	GLuint buffer;
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize, positions.data());
	glBufferSubData(GL_ARRAY_BUFFER, positionsSize, colorsSize, colors.data());

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_DYNAMIC_DRAW);

	// Set up arrays	
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, NUM_POSITIONS, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, NUM_COLORS, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(positionsSize));
}

Renderer Renderer:: subData(float t) {
	int num = t / indices.size();

	std::vector<GLfloat> p = positions;
	std::vector<GLfloat> c = colors;
	std::vector<GLint> i = indices;

	p.resize(num * NUM_POSITIONS);
	c.resize(num * NUM_COLORS);
	i.resize(num);
	
	return Renderer(p, c, i);	
}