//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include "OpenGL/Primitives/Primitive.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

//=============================================================================================================================================//
//  Constructor.																															   //
//=============================================================================================================================================//

template<typename VertexType>
Primitive<VertexType>::Primitive(Entity* parent) 
	: PrimitivePtr(parent) 
{}

//=============================================================================================================================================//
//  Memory.																																	   //
//=============================================================================================================================================//

template<typename VertexType>
Primitive<VertexType>::~Primitive() 
{ 
	wipeGPU(); 
}

template<typename VertexType>
void Primitive<VertexType>::wipeGPU()
{ 
	// Clear from the VAO primitive buffer.
	m_VAO->popPrimitive(this);
	// Clear metadata.
	m_vertexBufferPos = NULL;
	m_indexBufferPos = NULL;
	m_primitiveBufferPos = NULL;
	m_vertexCount = 0;
	m_indexCount = 0;
}

template<typename VertexType>
void Primitive<VertexType>::syncWithGPU() 
{
	m_VAO->syncPrimitiveVertexData(this);
}

//=============================================================================================================================================//
//  Manipulation.																															   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::translate(const glm::vec3& translation)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation;

	m_trackedCenter += translation;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translate(const glm::vec2& translation)
{
	Primitive::translate(glm::vec3{ translation, 0.f });
}

template<typename VertexType>
void Primitive<VertexType>::translateTo(const glm::vec3& position)
{ 
	glm::vec3 translation = position - m_trackedCenter; 
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation;

	m_trackedCenter += translation;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translateTo(const glm::vec2& position)
{
	Primitive::translateTo(glm::vec3{ position, m_trackedCenter.z });
}

template<typename VertexType>
void Primitive<VertexType>::rotate(float degrees, const glm::vec3& rotateNormal)
{
	Primitive::rotate(degrees, m_trackedCenter, rotateNormal);
}

template<typename VertexType>
void Primitive<VertexType>::rotate(float degrees, const glm::vec3& rotatePoint, const glm::vec3& rotateNormal)
{
	// Rather call transform if a lot of these are going to be called sequencially.
	glm::mat4 transform = glm::translate(glm::mat4(1.f), rotatePoint);
	transform = glm::rotate(transform, glm::radians(degrees), rotateNormal);
	transform = glm::translate(transform, -rotatePoint);

	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position = glm::vec3(transform * glm::vec4(m_VAO->m_vertexCPU[i].data.position, 1.f));

	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::transform(const glm::mat4& transfromMatrix)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position = glm::vec3(transfromMatrix * glm::vec4(m_VAO->m_vertexCPU[i].data.position, 1.f));

	// Not sure if this is correct (scaling?).
	m_trackedCenter = glm::vec3(transfromMatrix * glm::vec4(m_trackedCenter, 1.f));

	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::scale(const glm::vec3& scaling)
{
	// ...

	syncWithGPU();
}

//=============================================================================================================================================//
//  Set attributes.																															   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::setColor(const glm::vec4& color)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.color = color;

	m_colour = color;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::setEntityID(unsigned int eID)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].entityID = eID;

	m_entityID = eID;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::setLayer(float layer)
{
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position.z = layer;

	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::enableOutline()
{
	if (m_outlineEnabled) return;
	m_outlineEnabled = true;

	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.outline = 1.0f;

	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::disableOutline()
{
	if (!m_outlineEnabled) return;
	m_outlineEnabled = false;

	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.outline = 0.f;

	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translateVertexTo(VertexType* vertex, const glm::vec3 position)
{
	vertex->data.position = position;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translateVertexTo(VertexType* vertex, const glm::vec2 position)
{
	translateVertexTo(vertex, glm::vec3{ position, vertex->data.position.z });
}

template<typename VertexType>
void Primitive<VertexType>::translateVertex(VertexType* vertex, const glm::vec3 translation)
{
	vertex->data.position += translation;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translateVertex(VertexType* vertex, const glm::vec2 translation)
{
	Primitive::translateVertex(vertex, glm::vec3{ translation, 0.f });
}

//=============================================================================================================================================//
//  Vertex.																																	   //
//=============================================================================================================================================//

template<typename VertexType>
void Primitive<VertexType>::translateVertexAtIndex(unsigned index, const glm::vec3& translation) 
{
	m_VAO->m_vertexCPU[m_vertexBufferPos + index].data.position += translation;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translateVertexAtIndex(unsigned index, const glm::vec2& translation) 
{
	translateVertexAtIndex(index, {translation.x, translation.y, 0.f});
}

template<typename VertexType>
void Primitive<VertexType>::translateToVertexAtIndex(unsigned index, const glm::vec3& position)
{
	m_VAO->m_vertexCPU[m_vertexBufferPos + index].data.position = position;
	syncWithGPU();
}

template<typename VertexType>
void Primitive<VertexType>::translateToVertexAtIndex(unsigned index, const glm::vec2& position)
{
	translateToVertexAtIndex(index, { position.x, position.y, 0.f });
}

template<typename VertexType>
void Primitive<VertexType>::setVertexColorAtIndex(unsigned index, const glm::vec4& color) 
{
	m_VAO->m_vertexCPU[m_vertexBufferPos + index].data.color = color;
	syncWithGPU();
}

template<typename VertexType>
std::tuple<VertexType*, float> Primitive<VertexType>::getNearestVertex(const glm::vec3& position)
{
	float minDistance = 0.f;
	VertexType* closestVertex;
	// Calculate the first vertex' distance.
	minDistance = glm::abs(glm::distance(position, m_VAO->m_vertexCPU[m_vertexBufferPos].data.position));
	closestVertex = &m_VAO->m_vertexCPU[m_vertexBufferPos];
	// Find if any of the vertices are closer.
	for (int i = m_vertexBufferPos+1; i < m_vertexBufferPos + m_vertexCount; i++)
	{
		float currentDistance = glm::abs(glm::distance(position, m_VAO->m_vertexCPU[i].data.position));
		if (currentDistance > minDistance)
			continue;
		closestVertex = &m_VAO->m_vertexCPU[i];
		minDistance = currentDistance;
	}
	// Return the closes vertex, alongside the distance in world coordinates.
	return { closestVertex, minDistance };
}

template<typename VertexType>
std::tuple<VertexType*, float> Primitive<VertexType>::getNearestVertex(const glm::vec2& position)
{
	float minDistance = 0.f;
	VertexType* closestVertex;
	// Calculate the first vertex' distance.
	minDistance = glm::abs(glm::distance(position, glm::vec2(m_VAO->m_vertexCPU[m_vertexBufferPos].data.position)));
	closestVertex = &m_VAO->m_vertexCPU[m_vertexBufferPos];
	// Find if any of the vertices are closer.
	for (int i = m_vertexBufferPos + 1; i < m_vertexBufferPos + m_vertexCount; i++)
	{
		float currentDistance = glm::abs(glm::distance(position, glm::vec2(m_VAO->m_vertexCPU[i].data.position)));
		if (currentDistance > minDistance)
			continue;
		closestVertex = &m_VAO->m_vertexCPU[i];
		minDistance = currentDistance;
	}
	// Return the closes vertex, alongside the distance in world coordinates.
	return { closestVertex, minDistance };
}

template<typename VertexType>
std::tuple<unsigned, float> Primitive<VertexType>::getNearestVertexIdx(const glm::vec3& position)
{
	float minDistance = 0.f;
	unsigned closestVertexIdx;
	// Calculate the first vertex' distance.
	minDistance = std::numeric_limits<float>::infinity();
	closestVertexIdx = -1;
	// Find if any of the vertices are closer.
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
	{
		float currentDistance = glm::abs(glm::distance(position, m_VAO->m_vertexCPU[i].data.position));
		if (currentDistance > minDistance)
			continue;
		closestVertexIdx = i-m_vertexBufferPos;
		minDistance = currentDistance;
	}
	// Return the closes vertex, alongside the distance in world coordinates.
	return { closestVertexIdx, minDistance };
}

template<typename VertexType>
std::tuple<unsigned, float> Primitive<VertexType>::getNearestVertexIdx(const glm::vec2& position)
{
	float minDistance = 0.f;
	unsigned closestVertexIdx;
	// Calculate the first vertex' distance.
	minDistance = std::numeric_limits<float>::infinity();
	closestVertexIdx = -1;
	// Find if any of the vertices are closer.
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
	{
		float currentDistance = glm::abs(glm::distance(position, glm::vec2(m_VAO->m_vertexCPU[i].data.position)));
		if (currentDistance > minDistance)
			continue;
		closestVertexIdx = i - m_vertexBufferPos;
		minDistance = currentDistance;
	}
	// Return the closes vertex, alongside the distance in world coordinates.
	return { closestVertexIdx, minDistance };
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