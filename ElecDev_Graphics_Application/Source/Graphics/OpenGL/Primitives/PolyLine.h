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
    
    PolyLine(std::vector<glm::vec2> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent);
    //Add a new line segment
    void addSegment(glm::vec2 nextPoint);
    // Move a vertex
    void translateVertex(Circle* vertex, const glm::vec2 translation);


};

