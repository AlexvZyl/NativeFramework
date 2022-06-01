//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/Vertex.h"
#include "Graphics/Entities/EntityManager.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Circle::Circle(VertexArrayObject<VertexDataCircle>* VAO, const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Primitive<VertexDataCircle>(parent)
{
	// Init.
	m_VAO = VAO;
	m_trackedCenter = center;
	m_colour = color;
	m_thickness = thickness;
	m_fade = fade;
	m_radius = radius;

	// Vertices.
	VertexDataCircle vertices[4] = {
		{ { center.x - radius, center.y + radius, center.z }, local1, m_radius, m_colour, m_thickness, m_fade, m_entityID },
		{ { center.x + radius, center.y + radius, center.z }, local2, m_radius, m_colour, m_thickness, m_fade, m_entityID },
		{ { center.x + radius, center.y - radius, center.z }, local3, m_radius, m_colour, m_thickness, m_fade, m_entityID },
		{ { center.x - radius, center.y - radius, center.z }, local4, m_radius, m_colour, m_thickness, m_fade, m_entityID }
	};	

	// Indices.
	UInd3 indices[2] = {
		{0, 1, 2},
		{2, 3, 0}
	};

	// Add to VAO.
	m_VAO->pushPrimitive(this, vertices, indices);
} 

Circle::Circle(VertexArrayObject<VertexDataCircle>* vao, const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Circle( vao, glm::vec3(center, 0.f), radius, color, thickness, fade, parent)
{}

void Circle::setRadius(float radius)
{
	m_radius = radius;

	// Create the vertices.
	std::vector<VertexDataCircle> vertices;
	vertices.reserve(4);
	vertices.emplace_back(VertexDataCircle({ m_trackedCenter.x - radius, m_trackedCenter.y + radius, m_trackedCenter.z }, local1, m_radius, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(VertexDataCircle({ m_trackedCenter.x + radius, m_trackedCenter.y + radius, m_trackedCenter.z }, local2, m_radius, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(VertexDataCircle({ m_trackedCenter.x + radius, m_trackedCenter.y - radius, m_trackedCenter.z }, local3, m_radius, m_colour, m_thickness, m_fade, m_entityID));
	vertices.emplace_back(VertexDataCircle({ m_trackedCenter.x - radius, m_trackedCenter.y - radius, m_trackedCenter.z }, local4, m_radius, m_colour, m_thickness, m_fade, m_entityID));

	// Create the indices.
	std::vector<unsigned> indices = { 0,1,2,2,3,0 };

	// Add to VAO.
	m_VAO->popPrimitive(this);
	m_VAO->pushPrimitive(this, vertices, indices);
}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
