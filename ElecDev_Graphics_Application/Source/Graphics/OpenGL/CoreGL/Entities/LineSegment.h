#pragma once
#include "Entity.h"
#include "Vertex.h"
#include <memory>
class LineSegment :
    public Entity<VertexData>
{
private:


public:

    LineSegment(VertexData start, VertexData end,  float thickness, VertexArrayObject<VertexData>* VAO, ManagedEntity* parent);

};

