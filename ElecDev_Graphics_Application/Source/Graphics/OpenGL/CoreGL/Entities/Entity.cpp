//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/VertexArrayObjectGL.h"
#include "Vertex.h"
#include "Entity.h"
#include <vector>
#include <glm.hpp>

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

Entity::~Entity() 
{
	m_VAO->deleteDataCPU(this); m_VAO->updateGPU();		// Clear the data from the GPU.
	m_vertices.clear(); m_vertices.shrink_to_fit();		// Clear the data from the CPU.
}

void Entity::destroy() 
{
	this->~Entity();
}

void Entity::translate(glm::vec3* translation)
{
	for (Vertex* vertex : m_vertices) { vertex->position += *translation; }
	m_trackedCenter += *translation;
}

void Entity::translateTo(glm::vec3* position) 
{ 
	glm::vec3 translation = *position - m_trackedCenter; 
	for (Vertex* vertex : m_vertices) { vertex->position += translation; }
	m_trackedCenter += translation;
}

void Entity::rotate(glm::vec3* rotation)
{

}

void Entity::scale(glm::vec3* scaling)
{

}

void Entity::setColor(glm::vec4* color) 
{
	for (Vertex* vertex : m_vertices) { vertex->color = *color; }
	m_colour = *color;
}

void Entity::setEntityID(unsigned int eID) 
{
	for (Vertex* vertex : m_vertices) { vertex->entityID = eID; }
	m_entityID = eID;
}

void Entity::update() 
{
	m_VAO->assignDataGPU(this); 
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//