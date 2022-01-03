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

Polygon2D::Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent) 
	: Primitive<VertexData>(parent)
{
	// General setup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	m_vertexCount = vertices.size();
	m_VAO = VAO;
	std::vector<VertexData> vertexVector;
	std::vector<unsigned> indices;

	// Create the vertices.
	for (glm::vec3 vertex : vertices)
	{
		vertexVector.push_back(VertexData(vertex, m_colour, m_entityID));
	}

	// Assign the indices based on the VAO type.
	if (m_VAO->m_bufferType == GL_TRIANGLES)
	{
		indices = { 0,1,2,2,3,0 };
		m_indexCount = 6;
	}
	else if (m_VAO->m_bufferType == GL_LINES)
	{
		indices = { 0,1,1,2,2,3,3,0 };
		m_indexCount = 8;
	}

	// Pass to VAO.
	m_VAO->appendVertexData(vertexVector, indices, &m_vertexBufferPos, &m_indexBufferPos);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
