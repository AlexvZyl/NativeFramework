#pragma once
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"
#include <Clipper/cpp/clipper.hpp>

class VertexData;

class PolyLine : public Polygon2D
{

public:

    float m_thickness = 0.014f;
    // We need to retain a copy of the raw vertices for editing.
    std::vector<glm::vec2> m_vertices;
	float m_layer = 0.f;
	bool outlined = false;
	bool m_closed = false;
	bool m_rounded = true;

	ClipperLib::EndType et = ClipperLib::etOpenRound;
    
    PolyLine(std::vector<glm::vec2> vertices, GraphicsTrianglesBuffer<VertexData>* gtb, Entity* parent, float thickness = 0.014f, bool closed = false, glm::vec4 colour = {0.f, 0.f, 0.f, 1.f}, bool rounded = true);
    //Calculates the offset and runs the triangulation function
	virtual void pushVertex(const glm::vec3& vertex) override;
	virtual void pushVertex(const glm::vec2& vertex);
	virtual void translateVertexAtIndex(const unsigned& index, const glm::vec3& translation) override;
	virtual void translateVertexAtIndex(const unsigned& index, const glm::vec2& translation) override;
	// Translate to.
	virtual void translateVertexAtIndexTo(const unsigned& index, const glm::vec3& position) override;
	virtual void translateVertexAtIndexTo(const unsigned& index, const glm::vec2& position) override;
	// Move a vertex
	virtual void translateVertexTo(VertexData* vertex, const glm::vec3 position) override;
	// Move a vertex
	virtual void translateVertexTo(VertexData* vertex, const glm::vec2 position) override;
	// Move a vertex
	virtual void translateVertex(VertexData* vertex, const glm::vec3 translation) override;
	// Move a vertex
	virtual void translateVertex(VertexData* vertex, const glm::vec2 translation) override;
	// Translate the entity by the given vector.
	virtual void translate(const glm::vec3& translation) override;
	// Translate the entity by the given vector.
	virtual void translate(const glm::vec2& translation) override;
	// Translate the entity to the given position.
	virtual void translateTo(const glm::vec3& position) override;
	//Translates the entity in the XY plane, keeping the same Z value.
	virtual void translateTo(const glm::vec2& position) override;
	virtual void enableOutline(float value = 1.f) override;
	virtual void disableOutline() override;
	virtual void setThickness(float thickness);

	virtual std::tuple<unsigned, float> getNearestVertexIndex(const glm::vec2& position) override;

protected:

	void update();

};

