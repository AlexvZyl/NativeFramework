//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/VertexArrayObjectGL.h"
#include "Vertex.h"
#include "Entity.h"
#include <vector>
#include <glm.hpp>
#include "../GUI/GUIState.h"

//=============================================================================================================================================//
//  Constructor and Deconstructor.																											   //
//=============================================================================================================================================//

template<typename VertexType>
Entity<VertexType>::Entity() {}

template<typename VertexType>
Entity<VertexType>::Entity(ManagedEntity* parent) : ManagedEntity(parent) {}

template<typename VertexType>
Entity<VertexType>::~Entity() 
{
	// Clear the data from the GPU.
	m_VAO->deleteDataCPU(this);		
	// Clear the data from the CPU.
	m_vertices.clear(); 
	m_vertices.shrink_to_fit();		
	m_VAO->updateGPU();		
}

template<typename VertexType>
void Entity<VertexType>::wipeMemory() 
{ 
	// Clear the data from the GPU.
	m_VAO->deleteDataCPU(this);		
	m_VAO->updateGPU();
	// Clear the data from the CPU.
	m_vertices.clear();
	m_vertices.shrink_to_fit();		
	/*this->~Entity(); */
}

//=============================================================================================================================================//
//  Memory management.																														   //
//=============================================================================================================================================//

template<typename VertexType>
void Entity<VertexType>::offsetIndices(int offset) 
{
	for (unsigned& index : m_indices) { index += offset; }
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

template<typename VertexType>
void Entity<VertexType>::update()
{ 
	m_VAO->assignDataGPU(this); 
}

//=============================================================================================================================================//
//  Movement.																																   //
//=============================================================================================================================================//

template<typename VertexType>
void Entity<VertexType>::translate(glm::vec3& translation)
{
	for (VertexType& vertex : m_vertices) { vertex.data.position += translation; }
	m_trackedCenter += translation;
}

template<typename VertexType>
void Entity<VertexType>::translateTo(glm::vec3& position)
{ 
	glm::vec3 translation = position - m_trackedCenter; 
	for (VertexType& vertex : m_vertices) { vertex.data.position += translation; }
	m_trackedCenter += translation;
}

template<typename VertexType>
void Entity<VertexType>::translateTo(glm::vec2& position)
{
	glm::vec3 translation = glm::vec3(position, m_trackedCenter.z) - m_trackedCenter;
	for (VertexType& vertex : m_vertices) { vertex.data.position += translation; }
	m_trackedCenter += translation;
}

template<typename VertexType>
void Entity<VertexType>::rotate(glm::vec3& rotation)
{

}

template<typename VertexType>
void Entity<VertexType>::scale(glm::vec3& scaling)
{

}

//=============================================================================================================================================//
//  Set attributes.																															   //
//=============================================================================================================================================//

template<typename VertexType>
void Entity<VertexType>::setColor(glm::vec4& color)
{
	for (VertexType& vertex : m_vertices) { vertex.data.color = color; }
	m_colour = color;
}

template<typename VertexType>
void Entity<VertexType>::setEntityID(unsigned int eID)
{
	for (VertexType& vertex : m_vertices) { vertex.entityID = eID; }
	m_entityID = eID;
}

template<typename VertexType>
void Entity<VertexType>::setLayer(float layer)
{
	for (VertexType& vertex : m_vertices) { vertex.data.position.z = layer; }
}

template<typename VertexType>
void Entity<VertexType>::setContext(GUIState* guiState)
{
	guiState->clickedZone.primative = true;
	if (m_parent != nullptr) {
		m_parent->setContext(guiState);
	}
}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Entity<VertexData>;
template class Entity<VertexDataTextured>;
template class Entity<VertexDataCircle>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//