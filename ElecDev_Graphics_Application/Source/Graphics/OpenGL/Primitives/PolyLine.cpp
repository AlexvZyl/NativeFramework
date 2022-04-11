#include "PolyLine.h"
#include "CavalierContours/include/cavc/polylineoffset.hpp"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include <Clipper/cpp/clipper.hpp>

PolyLine::PolyLine(std::vector<glm::vec2> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent) :Polygon2D({}, VAO, parent)
{
	/*
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

	ClipperLib::Path subj; 
	ClipperLib::Paths solution;
	unsigned sf = 1000;
	std::transform(vertices.begin(), vertices.end(), std::back_inserter(subj), [&](glm::vec2 in) {
		return ClipperLib::IntPoint(static_cast<ClipperLib::cInt>(in.x*sf), static_cast<ClipperLib::cInt>(in.y*sf));
		});
	ClipperLib::ClipperOffset co;
	co.AddPath(subj, ClipperLib::jtSquare, ClipperLib::etOpenSquare);
	co.Execute(solution, m_thickness/2 * sf);

	std::vector<std::vector<glm::vec3>> resultVec;
	for (ClipperLib::Path path : solution) {
		std::vector<glm::vec3> temp;
		std::transform(path.begin(), path.end(), std::back_inserter(temp), [&](ClipperLib::IntPoint in) {
			return glm::vec3(in.X/sf, in.Y/sf , 0.f);
			});
		resultVec.push_back(temp);
	}

	m_VAO->popPrimitive(this);
	std::vector<unsigned> indices = mapbox::earcut<unsigned>(resultVec);
	m_vertexCount = resultVec[0].size();
	m_indexCount = indices.size();




	std::vector<VertexData> vertexVector;
	for (auto& vertex : resultVec[0])
		vertexVector.emplace_back(VertexData(vertex, m_colour, m_entityID));
	m_VAO->pushPrimitive(this, vertexVector, indices);
}

void PolyLine::addSegment(glm::vec2 nextPoint)
{
}

void PolyLine::translateVertex(Circle* vertex, const glm::vec2 translation)
{
}
