#pragma once
#include "CoreGL/Entities/LineSegment.h"
#include "CoreGL/Entities/Entity.h"
#include "Port.h"
#include "Circuit.h"
/*
#include "CoreGL/VertexArrayObjectGL.h"
#include <glm.hpp>
#include <vector>*/
//#include "CoreGL/Entities/Vertex.h"

enum class LineOrientation
{
    HORIZONTAL,
    VERTICAL
};

class Cable :
    public Entity
{
private:
    Port* m_startPort;
    Port* m_endPort;
    VertexArrayObject<VertexData>* engine_VAO;
    //std::vector<glm::vec2> m_vertices;
    std::vector<std::shared_ptr<LineSegment>> m_lines;
    LineOrientation m_curOrientation;


public:
    
    Cable(Port* startPort, VertexArrayObject<VertexData>* VAO, Circuit* parent);
    void addSegment(glm::vec2 nextPoint);
    void setContext(GUIState* guiState);
    
    

};

