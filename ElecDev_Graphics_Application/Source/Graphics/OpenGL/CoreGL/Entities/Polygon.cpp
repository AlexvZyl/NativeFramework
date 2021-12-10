//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Vertex.h"
#include "Polygon.h"
#include "EntityManager.h"
#include "CoreGL/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																										       	   //
//=============================================================================================================================================//

template<typename VertexType>
Polygon2D<VertexType>::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject<VertexType>* VAO, ManagedEntity* parent) 
	: Primitive<VertexType>(parent)
{
	// General setup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	m_vertexCount = vertices.size();
	m_VAO = VAO;

	// Create the vertices.
	for (glm::vec3 vertex : vertices)
		m_vertices.push_back(VertexData(vertex, m_colour, m_entityID));

	// Assign the indices based on the VAO type.
	if (m_VAO->m_bufferType == GL_TRIANGLES)
	{
		m_indices = { 0,1,2,2,3,0 };
		m_indexCount = 6;
	}
	else if (m_VAO->m_bufferType == GL_LINES)
	{
		m_indices = { 0,1,1,2,2,3,3,0 };
		m_indexCount = 8;
	}
	// Pass to VAO.
	m_VAO->appendDataCPU(this);
	m_VAO->updateGPU();
}

template<typename VertexType>
Polygon2D<VertexType>::~Polygon2D(){}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Polygon2D<VertexData>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
