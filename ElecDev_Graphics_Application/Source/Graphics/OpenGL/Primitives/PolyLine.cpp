#include "PolyLine.h"

PolyLine::PolyLine(std::vector<glm::vec2> vertices, Entity* parent) :Entity(EntityType::PRIMITIVE, parent) {

	m_nodes.emplace_back(Renderer::addCircle2D(vertices[0], m_thickness, m_colour, 1.f, 0.f, this));
	for (int i = 1; i < vertices.size(); i++)
	{
		m_lines.emplace_back(Renderer::addLineSegment2D(vertices[i - 1], vertices[i], m_thickness, m_colour, this));
		m_nodes.emplace_back(Renderer::addCircle2D(vertices[i], m_thickness, m_colour, 1.f, 0.f, this));
	}
}

void PolyLine::addSegment(glm::vec2 nextPoint)
{
}
