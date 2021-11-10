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

void Entity::translate(glm::vec3 translation)
{
	for (std::shared_ptr<Vertex> vertex : m_vertices) { vertex->position += translation; }
	m_trackedCenter += translation;
}

void Entity::translateTo(glm::vec3 position) 
{ 
	translate(glm::vec3(position - m_trackedCenter)); 
}

void Entity::rotate(glm::vec3 rotation)
{

}

void Entity::scale(glm::vec3 scaling)
{

}

void Entity::setColor(glm::vec4 color) 
{
	for (std::shared_ptr<Vertex> vertex : m_vertices) { vertex->color = color; }
	m_colour = color;
}

void Entity::setEntityID(unsigned int eID) 
{
	for (std::shared_ptr<Vertex> vertex : m_vertices) { vertex->entityID = eID; }
	m_entityID = eID;
}

void Entity::destroy() 
{
	// Clear the data from the GPU.
	m_VAO->deleteDataCPU(this); m_VAO->updateGPU();
	// Clear the data from the CPU.
	m_vertices.clear(); m_vertices.shrink_to_fit();
	//this->~Entity()
}

void Entity::update() 
{
	m_VAO->assignDataGPU(this); 
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//