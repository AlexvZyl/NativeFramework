//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/VertexArrayObjectGL.h"
#include "Vertex.h"
#include "Primitive.h"
#include <vector>
#include <glm.hpp>
#include "../GUI/GUIState.h"
#include "CoreGL/Renderer.h"

//=============================================================================================================================================//
//  Constructor and Deconstructor.																											   //
//=============================================================================================================================================//

template<typename VertexType>
Primitive<VertexType>::Primitive(Entity* parent) : PrimitivePtr(parent) {}

template<typename VertexType>
Primitive<VertexType>::~Primitive() 
{ 
	wipeGPU(); 
}

template<typename VertexType>
void Primitive<VertexType>::wipeGPU()
{ 
	// Clear from the VAO primitive buffer.
	m_VAO->popPrimitive(m_primitiveBufferPos, m_vertexCount, m_indexCount);
	// Clear vertex and index data.
	m_VAO->deleteVertexData(m_vertexBufferPos, m_vertexCount, m_indexBufferPos, m_indexCount);
	// Clear metadata.
	m_vertexBufferPos = NULL;
	m_indexBufferPos = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
}

//=============================================================================================================================================//
//  Movement.																																   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::translate(glm::vec3& translation)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation;
	m_trackedCenter += translation;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::translate(glm::vec2& translation)
{
	glm::vec3 translation3{ translation, 0.f };
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation3;
	m_trackedCenter += translation3;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::translateTo(glm::vec3& position)
{ 
	glm::vec3 translation = position - m_trackedCenter; 
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation;
	m_trackedCenter += translation;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::translateTo(glm::vec2& position)
{
	glm::vec3 translation = glm::vec3(position, m_trackedCenter.z) - m_trackedCenter;
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation;
	m_trackedCenter += translation;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::rotate(glm::vec3& rotation)
{
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::scale(glm::vec3& scaling)
{
	m_VAO->sync();
}

//=============================================================================================================================================//
//  Set attributes.																															   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::setColor(glm::vec4& color)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.color = color;
	m_colour = color;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::setEntityID(unsigned int eID)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].entityID = eID;
	m_entityID = eID;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::setLayer(float layer)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position.z = layer;
	m_VAO->sync();
}

template<typename VertexType>
void Primitive<VertexType>::setContext(GUIState* guiState)
{
	guiState->clickedZone.primative = true;
	if (m_parent) m_parent->setContext(guiState);
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