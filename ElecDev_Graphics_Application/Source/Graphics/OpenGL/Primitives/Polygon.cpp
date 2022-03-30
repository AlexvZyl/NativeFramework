//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Primitives/Polygon.h"
#include "Graphics/Entities/EntityManager.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																										       	   //
//=============================================================================================================================================//

Polygon2D::Polygon2D(const std::vector<glm::vec3>& vertices, VertexArrayObject<VertexData>* VAO, Entity* parent) 
	: Primitive<VertexData>(parent)
{
	// Inits.
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	m_vertexCount = vertices.size();
	m_VAO = VAO;
	// Find centre.
	float xTot = 0, yTot = 0, zTot = 0;
	for (auto& vec : vertices)
	{
		xTot += vec.x;
		yTot += vec.y;
		zTot += vec.z;
	}
	m_trackedCenter = {xTot/m_vertexCount, yTot/m_vertexCount, zTot/m_vertexCount}; 

	std::vector<VertexData> vertexVector;
	vertexVector.reserve(m_vertexCount);
	std::vector<unsigned> indices;
	
	// ----------------- //
	//  V E R T I C E S  //
	// ----------------- //

	for (auto& vertex : vertices)
		vertexVector.emplace_back(VertexData(vertex, m_colour, m_entityID));

	// --------------- //
	//  I N D I C E S  //
	// --------------- //

	if (m_VAO->getBufferType() == GL_TRIANGLES)
	{
		indices.reserve(3 * (m_vertexCount - 2));
		for (int i = 2; i < m_vertexCount; i++) 
		{
			indices.push_back(0);
			indices.push_back(i-1);
			indices.push_back(i);
		}
	}
	else if (m_VAO->getBufferType() == GL_LINES)
	{
		indices.reserve(2 * m_vertexCount);
		for (int i = 1; i < m_vertexCount; i++) 
		{
			indices.push_back(i -1);
			indices.push_back(i);
		}
		indices.push_back(0);
		indices.push_back(m_vertexCount-1);
	}
	m_indexCount = indices.size();
    
	// Pass to VAO.
	m_VAO->pushPrimitive(this, vertexVector, indices);
}

void Polygon2D::pushVertex(VertexData& vertex) 
{
	// Create new vertex vector.
	std::vector<VertexData> currentVertices;
	// Copy the existing vertices.
	currentVertices.reserve(m_vertexCount + 1);
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		currentVertices.emplace_back(m_VAO->m_vertexCPU[i]);

	// Add new vertex.
	currentVertices.emplace_back(vertex);

	// Pop and push the primitive.
	m_VAO->popPrimitive(this);
	m_vertexCount++;
	std::vector<unsigned> indices;  // Do tesselation here.
	m_indexCount = indices.size();
	m_VAO->pushPrimitive(this, currentVertices, indices);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//