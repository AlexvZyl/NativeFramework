//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/VertexArrayObjectGL.h"
#include "Vertex.h"
#include "Primitive.h"
#include <vector>
#include <glm.hpp>
#include "../GUI/GUIState.h"

//=============================================================================================================================================//
//  Constructor and Deconstructor.																											   //
//=============================================================================================================================================//

template<typename VertexType>
Primitive<VertexType>::Primitive(Entity* parent) : Entity(EntityType::PRIMATIVE, parent) {}

template<typename VertexType>
Primitive<VertexType>::~Primitive()
{
	wipeMemory();
}

template<typename VertexType>
void Primitive<VertexType>::wipeMemory()
{ 
	// Clear the data from the GPU.
	m_VAO->deleteDataCPU(this);		
	// Clear the data from the CPU.
	m_vertices.clear();
	m_vertices.shrink_to_fit();		
	m_indices.clear();
	m_indices.shrink_to_fit();
	m_indecesStartIndex = 0;
	m_bufferStartIndex = 0;
	m_vertexCount = 0;
	m_indexCount = 0;
}

//=============================================================================================================================================//
//  Memory management.																														   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::offsetIndices(int offset)
{
	for (unsigned& index : m_indices) { index += offset; }
	m_VAO->outOfSync();
}

//=============================================================================================================================================//
//  Movement.																																   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::translate(glm::vec3& translation)
{
	for (VertexType& vertex : m_vertices) { vertex.data.position += translation; }
	m_trackedCenter += translation;
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::translateTo(glm::vec3& position)
{ 
	glm::vec3 translation = position - m_trackedCenter; 
	for (VertexType& vertex : m_vertices) { vertex.data.position += translation; }
	m_trackedCenter += translation;
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::translateTo(glm::vec2& position)
{
	glm::vec3 translation = glm::vec3(position, m_trackedCenter.z) - m_trackedCenter;
	for (VertexType& vertex : m_vertices) { vertex.data.position += translation; }
	m_trackedCenter += translation;
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::rotate(glm::vec3& rotation)
{
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::scale(glm::vec3& scaling)
{
	m_VAO->outOfSync();
}

//=============================================================================================================================================//
//  Set attributes.																															   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::setColor(glm::vec4& color)
{
	for (VertexType& vertex : m_vertices) { vertex.data.color = color; }
	m_colour = color;
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::setEntityID(unsigned int eID)
{
	for (VertexType& vertex : m_vertices) { vertex.entityID = eID; }
	m_entityID = eID;
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::setLayer(float layer)
{
	for (VertexType& vertex : m_vertices) { vertex.data.position.z = layer; }
	m_VAO->outOfSync();
}

template<typename VertexType>
void Primitive<VertexType>::setContext(GUIState* guiState)
{
	guiState->clickedZone.primative = true;
	if (m_parent != nullptr) { m_parent->setContext(guiState); }
	m_VAO->outOfSync();
}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Primitive<VertexData>;
template class Primitive<VertexDataTextured>;
template class Primitive<VertexDataCircle>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//