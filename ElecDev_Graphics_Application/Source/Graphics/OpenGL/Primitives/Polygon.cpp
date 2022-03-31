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

	//Auto-centering code can be found below. This may be useful at some stage, but should not be default.
	/*/
	// Find centre.
	float xTot = 0, yTot = 0, zTot = 0;
	for (auto& vec : vertices)
	{
		xTot += vec.x;
		yTot += vec.y;
		zTot += vec.z;
	}
	m_trackedCenter = {xTot/m_vertexCount, yTot/m_vertexCount, zTot/m_vertexCount}; 
	*/
	
	//Centre on the origin
	m_trackedCenter = { 0.f, 0.f, 0.f };

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
		//tesselate with earcut.
		//NOTE: This is only valid for polygons in the XY plane. We need to project the points onto a 2D plane to support polygons in arbitrary planes.
		std::vector < std::vector<glm::vec3>> vertices_with_holes;
		vertices_with_holes.push_back(vertices);
		indices = mapbox::earcut<unsigned>(vertices_with_holes);

		/*//Old index code
		indices.reserve(3 * (m_vertexCount - 2));
		for (int i = 2; i < m_vertexCount; i++) 
		{
			indices.push_back(0);
			indices.push_back(i-1);
			indices.push_back(i);
		}

		*/
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

void Polygon2D::pushVertex(const glm::vec3& vertex) 
{
	// Create new vertex vector.
	std::vector<VertexData> currentVertices;
	std::vector<glm::vec3> vertices;
	// Copy the existing vertices.
	currentVertices.reserve(m_vertexCount + 1);
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++) {
		currentVertices.emplace_back(m_VAO->m_vertexCPU[i]);
		vertices.emplace_back((currentVertices.back().data.position));
	}
	// Add new vertex.
	currentVertices.emplace_back(VertexData(vertex, m_colour, m_entityID));
	vertices.emplace_back(vertex);

	// Pop and push the primitive.
	m_VAO->popPrimitive(this);
	m_vertexCount++;
	std::vector < std::vector<glm::vec3>> vertices_with_holes;
	vertices_with_holes.push_back(vertices);
	std::vector<unsigned> indices = mapbox::earcut<unsigned>(vertices_with_holes);
	m_indexCount = indices.size();
	m_VAO->pushPrimitive(this, currentVertices, indices);
}

void Polygon2D::translateVertexAtIndex(unsigned index, const glm::vec3& translation)
{
	m_VAO->m_vertexCPU[m_vertexBufferPos + index].data.position += translation;
	updateIndices();
	syncWithGPU();
}

void Polygon2D::translateVertexAtIndex(unsigned index, const glm::vec2& translation)
{
	translateVertexAtIndex(index, { translation.x, translation.y, 0.f });
}

void Polygon2D::translateToVertexAtIndex(unsigned index, const glm::vec3& position)
{
	glm::vec3* currentPosition = &m_VAO->m_vertexCPU[m_vertexBufferPos + index].data.position;
	*currentPosition += (position - *currentPosition);
	updateIndices();
	syncWithGPU();
}

void Polygon2D::translateToVertexAtIndex(unsigned index, const glm::vec2& position)
{
	translateToVertexAtIndex(index, { position.x, position.y, 0.f });
}

void Polygon2D::updateIndices()
{
	std::vector<glm::vec3> vertices;
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++) {
		vertices.emplace_back((m_VAO->m_vertexCPU[i].data.position));
	}
	std::vector < std::vector<glm::vec3>> vertices_with_holes;
	vertices_with_holes.push_back(vertices);
	std::vector<unsigned> indices = mapbox::earcut<unsigned>(vertices_with_holes);
	m_VAO->updateIndices(this, indices);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//