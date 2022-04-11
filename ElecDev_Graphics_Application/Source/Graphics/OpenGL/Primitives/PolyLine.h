#pragma once
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Logger/Logger.h"
class VertexData;


class PolyLine :
    public Polygon2D
{

    public:
    float m_thickness = 0.005f;
    //we need to retain a copy of the raw vertices for editing
    std::vector<glm::vec2> m_vertices;
	float m_layer = 0.f;
    
    PolyLine(std::vector<glm::vec2> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent);
    //Calculates the offset and runs the triangulation function
    void update();
	virtual void pushVertex(const glm::vec3& vertex) override;
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



};

