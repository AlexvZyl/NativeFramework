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

Polygon2D::Polygon2D(const std::vector<glm::vec3>& vertices, GraphicsTrianglesBuffer<VertexData>* gtb, Entity* parent, glm::vec4 colour)
	: Primitive<GraphicsTrianglesBuffer<VertexData>>(parent)
{
	// Inits.
	m_colour = colour;
	m_vertexCount = vertices.size();
	setGraphicsBuffer(gtb);
	m_trackedCenter = { 0.f, 0.f, 0.f };

	// Auto-centering code can be found below. This may be useful at some stage, but should not be default.
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

	std::vector<VertexData> vertexVector;
	vertexVector.reserve(m_vertexCount);
	std::vector<unsigned> indices;
	
	// Vertices.
	for (auto& vertex : vertices)
		vertexVector.emplace_back(vertex, m_colour, m_entityID);

	// Indices.
	// Tesselate with earcut.
	// NOTE: This is only valid for polygons in the XY plane. We need to project the 
	// points onto a 2D plane to support polygons in arbitrary planes.
	std::vector<std::vector<glm::vec3>> vertices_with_holes;
	vertices_with_holes.push_back(vertices);
	indices = mapbox::earcut<unsigned>(vertices_with_holes);

	// Pass to buffer.
	pushToGraphicsBuffer(vertexVector.data(), vertexVector.size(), (UInt3*)indices.data(), indices.size() / UInt3::count());

	/*else if (getGraphicsBuffer().getVAO().getType() == GL_LINES)
	{
		indices.reserve(2 * m_vertexCount);
		for (int i = 1; i < m_vertexCount; i++) 
		{
			indices.push_back(i -1);
			indices.push_back(i);
		}
		indices.push_back(0);
		indices.push_back(m_vertexCount-1);
	}*/
}

void Polygon2D::pushVertex(const glm::vec3& vertex) 
{
	// Copy the existing vertices.
	std::vector<VertexData> currentVertices;
	std::vector<glm::vec3> vertices;
	currentVertices.reserve(m_vertexCount + 1);
	for (int i = 0; i < m_vertexCount; i++) 
	{
		currentVertices.emplace_back(getVertex(i));
		vertices.emplace_back((currentVertices.back().position));
	}
	// Add new vertex.
	currentVertices.emplace_back(vertex, m_colour, m_entityID);

	// Update the data.
	m_vertexCount++;
	vertices.emplace_back(vertex);
	std::vector<std::vector<glm::vec3>> vertices_with_holes;
	vertices_with_holes.push_back(vertices);
	auto indices = mapbox::earcut<unsigned>(vertices_with_holes);
	removeFromGraphicsBuffer();
	pushToGraphicsBuffer(currentVertices.data(), currentVertices.size(), (UInt3*)indices.data(), indices.size() / UInt3::count());

	/*else if (getGraphicsBuffer().getVAO().getType() == GL_LINES)
	{
		indices.reserve(2 * m_vertexCount);
		for (int i = 1; i < m_vertexCount; i++)
		{
			indices.push_back(i - 1);
			indices.push_back(i);
		}
		indices.push_back(0);
		indices.push_back(m_vertexCount - 1);
		pushToGraphicsBuffer(currentVertices.data(), currentVertices.size(), (UInt2*)indices.data(), indices.size() / UInt2::count());
	}*/
}

void Polygon2D::translateVertexAtIndex(unsigned index, const glm::vec3& translation)
{
	getVertex(index).position += translation;
	updateIndices();
	syncWithGPU();
}

void Polygon2D::translateVertexAtIndex(unsigned index, const glm::vec2& translation)
{
	translateVertexAtIndex(index, { translation.x, translation.y, 0.f });
}

void Polygon2D::translateVertexAtIndexTo(unsigned index, const glm::vec3& position)
{
	glm::vec3* currentPosition = &getVertex(index).position;
	*currentPosition += (position - *currentPosition);
	updateIndices();
	syncWithGPU();
}

void Polygon2D::translateVertexAtIndexTo(unsigned index, const glm::vec2& position)
{
	translateVertexAtIndexTo(index, { position.x, position.y, 0.f });
}

void Polygon2D::updateIndices()
{
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < m_vertexCount; i++) 
		vertices.emplace_back((getVertex(i).position));
	std::vector<std::vector<glm::vec3>> vertices_with_holes;
	vertices_with_holes.push_back(vertices);
	auto indices = mapbox::earcut<unsigned>(vertices_with_holes);
	This::updateIndices((UInt3*)indices.data(), indices.size() / UInt3::count());

	/*else if (getGraphicsBuffer().getVAO().getType() == GL_LINES)
	{
		indices.reserve(2 * m_vertexCount);
		for (int i = 1; i < m_vertexCount; i++)
		{
			indices.push_back(i - 1);
			indices.push_back(i);
		}
		indices.push_back(0);
		indices.push_back(m_vertexCount - 1);
	}*/
}

void Polygon2D::translateVertexTo(VertexData* vertex, const glm::vec3 position)
{
	vertex->position = position;
	updateIndices();
	syncWithGPU();
}

void Polygon2D::translateVertexTo(VertexData* vertex, const glm::vec2 position)
{
	translateVertexTo(vertex, glm::vec3{ position, vertex->position.z });
}

void Polygon2D::translateVertex(VertexData* vertex, const glm::vec3 translation)
{
	translateVertexTo(vertex, vertex->position + translation);
}

void Polygon2D::translateVertex(VertexData* vertex, const glm::vec2 translation)
{
	translateVertex(vertex, glm::vec3{ translation, 0.f });
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//