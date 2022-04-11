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
	m_lines.emplace_back(Renderer::addLineSegment2D(m_lines.back()->m_end, nextPoint, m_thickness, m_colour, this));
	m_nodes.emplace_back(Renderer::addCircle2D(nextPoint, m_thickness, m_colour, 1.f, 0.f, this));
}

void PolyLine::translateVertex(Circle* vertex, const glm::vec2 translation)
{
	auto node = std::find_if(begin(m_nodes), end(m_nodes), [&](std::shared_ptr<Circle> current)
		{
			return current.get() == vertex;
		});
	if (node == end(m_nodes)) {
		//Circle is not in the list
		LUMEN_LOG_WARN("Tried to move an invalid vertex.", "")
		return;
	}
	auto idx = node - begin(m_nodes);
}
