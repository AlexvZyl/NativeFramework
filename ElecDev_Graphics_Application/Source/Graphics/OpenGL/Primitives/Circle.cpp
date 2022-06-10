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
	m_vertexCount = 4;

	// Indices.
	UInt3 indices[2] = {
		{0, 1, 2},
		{2, 3, 0}
	};
	m_indexCount = 2;

	// Add to VAO.
	auto [ m_vertexBufferPos, m_indexBufferPos ] = getGraphicsBuffer().push(vertices, 4, indices, 2);
} 

Circle::Circle(GraphicsTrianglesBuffer<VertexDataCircle>* gpb, const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent)
	: Circle( gpb, glm::vec3(center, 0.f), radius, color, thickness, fade, parent)
{ }

void Circle::setRadius(float radius)
{
	// Set new radius.
	m_radius = radius;

	// Update the vertexdata.
	getVertex(0).data.position = { m_trackedCenter.x - radius, m_trackedCenter.y + radius, m_trackedCenter.z };
	getVertex(1).data.position = { m_trackedCenter.x + radius, m_trackedCenter.y + radius, m_trackedCenter.z };
	getVertex(2).data.position = { m_trackedCenter.x + radius, m_trackedCenter.y - radius, m_trackedCenter.z };
	getVertex(3).data.position = { m_trackedCenter.x - radius, m_trackedCenter.y - radius, m_trackedCenter.z };

	// Sync.
	syncWithGPU();
}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
