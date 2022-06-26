//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "OpenGL/Primitives/Circle.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Buffers/GraphicsPrimitivesBuffersGL.h"
#include "Graphics/Entities/EntityManager.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

Circle::Circle(GraphicsTrianglesBuffer<VertexDataCircle>* gpb, const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Primitive<GraphicsTrianglesBuffer<VertexDataCircle>>(parent)
{
	// Init.
	setGraphicsBuffer(gpb);
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
	UInt3 indices[2] = {
		{0, 1, 2},
		{2, 3, 0}
	};

	// Add to buffer.
	pushToGraphicsBuffer(vertices, 4, indices, 2);
} 

Circle::Circle(GraphicsTrianglesBuffer<VertexDataCircle>* gpb, const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Circle( gpb, glm::vec3(center, 0.f), radius, color, thickness, fade, parent)
{ }

void Circle::setRadius(float radius)
{
	m_radius = radius;

	// Update vertices.
	getVertex(0).position = { m_trackedCenter.x - radius, m_trackedCenter.y + radius, m_trackedCenter.z };
	getVertex(1).position = { m_trackedCenter.x + radius, m_trackedCenter.y + radius, m_trackedCenter.z };
	getVertex(2).position = { m_trackedCenter.x + radius, m_trackedCenter.y - radius, m_trackedCenter.z };
	getVertex(3).position = { m_trackedCenter.x - radius, m_trackedCenter.y - radius, m_trackedCenter.z };

	// Update radius.
	getVertex(0).radius = radius;
	getVertex(1).radius = radius;
	getVertex(2).radius = radius;
	getVertex(3).radius = radius;

	syncWithGPU();
}

void Circle::setThickness(float thickness)
{
	// Cannot set filled circle thickness.
	if (m_thickness == -1) return;

	m_thickness = thickness;

	// Update thickness.
	getVertex(0).thickness = thickness;
	getVertex(1).thickness = thickness;
	getVertex(2).thickness = thickness;
	getVertex(3).thickness = thickness;

	syncWithGPU();
}

void Circle::translateVertexAtIndexTo(const unsigned& localIndex, const glm::vec2& position)
{
	//localIndex can be ignored (circles have infinite logicalIndices)
	setRadius(glm::distance(position, { m_trackedCenter }));
}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
