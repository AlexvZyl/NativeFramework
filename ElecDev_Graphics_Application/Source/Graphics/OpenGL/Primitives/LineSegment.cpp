//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "LineSegment.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

LineSegment::LineSegment(const glm::vec2& start, const glm::vec2& end, VertexArrayObject<VertexData>* VAO, Entity* parent, float thickness, const glm::vec4& colour)
	:	m_direction((end - start) / (glm::length(glm::vec2(end - start)))),
		m_perpendicular(-m_direction::member.y, -m_direction::member.x),
		m_start(start), m_end(end), m_thickness(thickness),
		Polygon2D(std::vector<glm::vec3>
		{
			glm::vec3(start - thickness * (m_perpendicular::member), 0.f),
			glm::vec3(start + thickness * (m_perpendicular::member), 0.f),
			glm::vec3(end   + thickness * (m_perpendicular::member), 0.f),
			glm::vec3(end   - thickness * (m_perpendicular::member), 0.f)
		},
		VAO, parent)
{
	setColor(colour);
}

void LineSegment::translate(const glm::vec2& translation)
{
	glm::vec3 translation3{ translation, 0.f };
	for (int i = m_vertexBufferPos; i < m_vertexBufferPos + m_vertexCount; i++)
		m_VAO->m_vertexCPU[i].data.position += translation3;

	m_trackedCenter += translation3;
	m_start += translation;
	m_end += translation;
	syncWithGPU();
}

void LineSegment::setStart(const glm::vec2& start)
{
	m_start = start;
	m_direction::member = (m_end - m_start) / (glm::length(glm::vec2(m_end - m_start)));
	m_perpendicular::member = glm::vec2( - m_direction::member.y, -m_direction::member.x);
	m_VAO->m_vertexCPU[m_vertexBufferPos].data.position = glm::vec3(m_start - m_thickness * (m_perpendicular::member), 0.f);
	m_VAO->m_vertexCPU[m_vertexBufferPos + 1].data.position = glm::vec3(m_start + m_thickness * (m_perpendicular::member), 0.f);
	syncWithGPU();
}

void LineSegment::setEnd(const glm::vec2& end)
{
	m_end = end;
	m_direction::member = (m_end - m_start) / (glm::length(glm::vec2(m_end - m_start)));
	m_perpendicular::member = glm::vec2( - m_direction::member.y, -m_direction::member.x);
	m_VAO->m_vertexCPU[m_vertexBufferPos + 2].data.position = glm::vec3(m_end + m_thickness * (m_perpendicular::member), 0.f);
	m_VAO->m_vertexCPU[m_vertexBufferPos + 3].data.position = glm::vec3(m_end - m_thickness * (m_perpendicular::member), 0.f);
	syncWithGPU();
}

//==============================================================================================================================================//
// EOF.																																			//
//==============================================================================================================================================//