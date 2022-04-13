#pragma once
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"
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
    
    PolyLine(std::vector<glm::vec2> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent, bool closed = false);
    //Calculates the offset and runs the triangulation function
	virtual void pushVertex(const glm::vec3& vertex) override;
	virtual void pushVertex(const glm::vec2& vertex);
	virtual void translateVertexAtIndex(unsigned index, const glm::vec3& translation) override;
	virtual void translateVertexAtIndex(unsigned index, const glm::vec2& translation) override;
	// Translate to.
	virtual void translateToVertexAtIndex(unsigned index, const glm::vec3& position) override;
	virtual void translateToVertexAtIndex(unsigned index, const glm::vec2& position) override;
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
	virtual void enableOutline() override;
	virtual void disableOutline() override;

	virtual std::tuple<unsigned, float> getNearestVertexIdx(const glm::vec2& position);

protected:

	void update();

};

