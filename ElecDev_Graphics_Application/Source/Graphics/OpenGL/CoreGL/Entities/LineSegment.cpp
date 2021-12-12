#include "LineSegment.h"
#include "CoreGL/VertexArrayObjectGL.h"


LineSegment::LineSegment(glm::vec2 start, glm::vec2 end, VertexArrayObject<VertexData>* VAO, Entity* parent, float thickness, glm::vec4 colour) 
	:m_direction((end-start)/(glm::length(glm::vec2(end - start)))),
	m_perpendicular(- m_direction::member.y, -m_direction::member.x),
	m_start(start), m_end(end), m_thickness(thickness),
	Polygon2D<VertexData>(std::vector <glm::vec3>{	glm::vec3(start - thickness*(m_perpendicular::member), 0.f),
													glm::vec3(start + thickness * (m_perpendicular::member), 0.f),
													glm::vec3(end + thickness * (m_perpendicular::member), 0.f),
													glm::vec3(end - thickness * (m_perpendicular::member), 0.f)},
													VAO, parent)
{
	setColor(colour);
}

void LineSegment::translate(glm::vec2& translation)
{
	glm::vec3 translation3{ translation, 0.f };
	for (VertexData& vertex : m_vertices) { vertex.data.position += translation3; }
	m_trackedCenter += translation3;
	m_start += translation;
	m_end += translation;
	m_VAO->outOfSync();
}
