//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/Entities/Circle.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Entities/EntityManager.h"
#include "CoreGL/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Circle::Circle(VertexArrayObject<VertexDataCircle>* vao, glm::vec3 center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent)
	: Primitive<VertexDataCircle>(parent)
{
	// Init.
	m_VAO = vao;
	m_trackedCenter = center;
	m_colour = color;
	m_thickness = thickness;
	m_fade = fade;

	// Create position variables.
	glm::vec3 pos1(center.x - radius, center.y + radius, center.z);
	glm::vec3 pos2(center.x + radius, center.y + radius, center.z);
	glm::vec3 pos3(center.x + radius, center.y - radius, center.z);
	glm::vec3 pos4(center.x - radius, center.y - radius, center.z);

	// Create vertices and indices.
	std::vector<std::unique_ptr<VertexDataCircle>> vertices;
	vertices.reserve(4);
	vertices.emplace_back(std::make_unique<VertexDataCircle>(pos1, local1, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(std::make_unique<VertexDataCircle>(pos2, local2, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(std::make_unique<VertexDataCircle>(pos3, local3, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(std::make_unique<VertexDataCircle>(pos4, local4, m_colour, m_thickness, m_fade, m_entityID));
	std::vector<unsigned> indices = {0,1,2,2,3,0};
	m_vertexCount = 4;
	m_indexCount = 6;

	// Add to VAO.
	m_VAO->appendVertexData(vertices, indices, &m_vertexBufferPos, &m_indexBufferPos);
	m_VAO->pushPrimitive(this);
}

Circle::Circle(VertexArrayObject<VertexDataCircle>* vao, glm::vec2 center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent)
	: Circle( vao, glm::vec3(center, 0.f), radius, color, thickness, fade, parent)
{}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
