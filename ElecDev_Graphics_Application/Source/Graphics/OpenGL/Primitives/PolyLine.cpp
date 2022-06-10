#include "PolyLine.h"
#include "CavalierContours/include/cavc/polylineoffset.hpp"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"

PolyLine::PolyLine(std::vector<glm::vec2> vertices, GraphicsTrianglesBuffer<VertexData>* gtb, Entity* parent, float thickness, bool closed, glm::vec4 colour, bool rounded) 
	: Polygon2D({}, gtb, parent, colour), m_vertices(vertices), m_closed(closed), m_thickness(thickness)
{
	/*CVAC implementation (not working)
	std::vector<cavc::PlineVertex<float>> verts;
	std::transform(vertices.begin(), vertices.end(), std::back_inserter(verts), [&](glm::vec2 in) {
		return glm::vec3(in*1000.f, 0.f);
		});
	cavc::Polyline<float> input(verts);

	//+ve offset
	std::vector<cavc::Polyline<float>> results = cavc::parallelOffset(input, -m_thickness*500);
	//std::vector<cavc::Polyline<float>> resultsNeg = cavc::parallelOffset(input, -m_thickness * 500);
	std::vector<std::vector<glm::vec3>> resultVec;
	cavc::Polyline<float> path = results[1];
		std::vector<glm::vec3> temp;
		std::transform(path.vertexes().begin(), path.vertexes().end(), std::back_inserter(temp), [&](cavc::PlineVertex<float> in) {
			return glm::vec3(in.x()/1000, in.y()/1000, 0.f);
			});
		std::transform(vertices.rbegin(), vertices.rend(), std::back_inserter(temp), [&](glm::vec2 in) {
			return glm::vec3(in.x, in.y, 0.f);
			});
		resultVec.push_back(temp);
	//}


	/*-ve offset
	//std::vector<cavc::Polyline<float>> resultsNeg = cavc::parallelOffset(input, -m_thickness*500);
	for (cavc::Polyline<float> path : resultsNeg) {
		std::transform(path.vertexes().rend(), path.vertexes().rbegin(), std::back_inserter(temp), [&](cavc::PlineVertex<float> in) {
			return glm::vec3(in.x(), in.y(), 0.f);
			});
		resultVec.push_back(temp);
	}*/
	m_rounded = rounded;
	if (!m_rounded) et = ClipperLib::etOpenButt;
	update();
	setColor(m_colour);
}

void PolyLine::update()
{
	ClipperLib::Path subj;
	ClipperLib::Paths solution;
	float sf = 100000;
	std::transform(m_vertices.begin(), m_vertices.end(), std::back_inserter(subj), [&](glm::vec2 in) {
		return ClipperLib::IntPoint(static_cast<ClipperLib::cInt>(in.x * sf), static_cast<ClipperLib::cInt>(in.y * sf));
		});
	ClipperLib::ClipperOffset co;
	if (m_closed) et = ClipperLib::etClosedLine;
	co.AddPath(subj, ClipperLib::jtRound, et);
	co.Execute(solution, m_thickness / 2 * sf);

	std::vector<std::vector<glm::vec3>> resultVec;
	for (ClipperLib::Path& path : solution) {
		std::vector<glm::vec3> temp;
		std::transform(path.begin(), path.end(), std::back_inserter(temp), [&](ClipperLib::IntPoint in) {
			return glm::vec3(in.X / sf, in.Y / sf, m_layer);
			});
		resultVec.push_back(temp);
	}

	m_VAO->popPrimitive(this);
	std::vector<unsigned> indices = mapbox::earcut<unsigned>(resultVec);
	m_indexCount = indices.size();

	std::vector<VertexData> vertexVector;
	for (auto& path : resultVec) {
		for (auto& vertex : path)
			vertexVector.emplace_back(VertexData(vertex, m_colour, m_entityID));
	}

	m_vertexCount = vertexVector.size();
	m_VAO->pushPrimitive(this, vertexVector, indices);
	if (outlined) 
	{
		enableOutline();
	}
}

void PolyLine::pushVertex(const glm::vec3& vertex)
{
	m_vertices.push_back({ vertex });
	update();
}

void PolyLine::pushVertex(const glm::vec2& vertex)
{
	m_vertices.push_back(vertex);
	update();
}

void PolyLine::translateVertexAtIndex(unsigned index, const glm::vec3& translation)
{
	translateVertexAtIndex(index, glm::vec2{ translation });
}

void PolyLine::translateVertexAtIndex(unsigned index, const glm::vec2& translation)
{
	m_vertices.at(index) += translation;
	update();
}

void PolyLine::translateVertexAtIndexTo(unsigned index, const glm::vec3& position)
{
	translateVertexAtIndexTo(index, glm::vec2{ position });
}

void PolyLine::translateVertexAtIndexTo(unsigned index, const glm::vec2& position)
{
	glm::vec2 translation = position - m_vertices.at(index);
	translateVertexAtIndex(index, translation);
}

void PolyLine::translateVertexTo(VertexData* vertex, const glm::vec3 position)
{
	translateVertexTo(vertex, glm::vec2{ position });
}

void PolyLine::translateVertexTo(VertexData* vertex, const glm::vec2 position)
{
	float tol = m_thickness;
	auto it = std::find_if(begin(m_vertices), end(m_vertices), [&](glm::vec2 vert)
		{
			return glm::length(vert - glm::vec2{ vertex->data.position }) < tol;
		});
	if (it == end(m_vertices)) {
		LUMEN_LOG_WARN("Tried to move an invlaid vertex.", "PolyLine");
		return;
	}
	*it = position;
	update();
}

void PolyLine::translateVertex(VertexData* vertex, const glm::vec3 translation)
{
	translateVertex(vertex, glm::vec2{ translation });
}

void PolyLine::translateVertex(VertexData* vertex, const glm::vec2 translation)
{
	float tol = m_thickness*2;
	auto it = std::find_if(begin(m_vertices), end(m_vertices), [&](glm::vec2 vert)
		{
			return glm::length(vert - glm::vec2{ vertex->data.position }) < tol;
		});

	if (it == end(m_vertices)) 
	{
		LUMEN_LOG_WARN("Tried to move an invlaid vertex.", "PolyLine");
		return;
	}

	LUMEN_LOG_DEBUG("idx = " + std::to_string(it - m_vertices.begin()), "PolyLine");
	*it += translation;
	update();
}

void PolyLine::translate(const glm::vec3& translation)
{
	translate(glm::vec2(translation));
}

void PolyLine::translate(const glm::vec2& translation)
{
	Primitive::translate(translation);
	// Update the internal vertices.
	for (auto& vert : m_vertices) 
		vert += translation;
}

void PolyLine::translateTo(const glm::vec3& position)
{
	translateTo(glm::vec2(position));
}

void PolyLine::translateTo(const glm::vec2& position)
{
	translate(position - glm::vec2(m_trackedCenter));
}

void PolyLine::enableOutline()
{
	outlined = true;
	Polygon2D::enableOutline();
}

void PolyLine::disableOutline()
{
	outlined = false;
	Polygon2D::disableOutline();
}

std::tuple<unsigned, float> PolyLine::getNearestVertexIdx(const glm::vec2& position)
{
	unsigned i = 0;
	unsigned idx = 0;
	float min = INFINITY;
	for (glm::vec2 vert : m_vertices) {
		if (glm::length(vert - position) < min) {
			idx = i;
			min = glm::length(vert - position);
		}
		i++;
	}
	return { idx, min };
}