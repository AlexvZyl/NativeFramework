#include "LineSegment.h"


LineSegment::LineSegment(glm::vec2 start, glm::vec2 end, VertexArrayObject<VertexData>* VAO, ManagedEntity* parent, float thickness, glm::vec4 colour) 
	:m_direction((end-start)/(glm::length(glm::vec2(end - start)))),
	m_perpendicular(- m_direction::member.y, -m_direction::member.x)
	Polygon2D<VertexData>(std::vector <glm::vec3>{glm::vec3(start-)})
{

}