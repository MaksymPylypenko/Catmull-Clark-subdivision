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