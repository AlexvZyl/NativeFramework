#pragma once
#include "CoreGL/Entities/Entity.h"
#include "CoreGL/Entities/Primitive.h"

class Port;

enum PortType
{
    PORT_IN, PORT_OUT, PORT_INOUT
};

enum class PortPosition
{
    TOP, BOTTOM, LEFT, RIGHT
};

class IOIndicator :
    public Primitive<VertexData>
{
private:
    PortType m_portType;
    PortPosition m_portPosition;

public:

    IOIndicator(PortType type, PortPosition position, VertexArrayObject<VertexData>* VAO, Port* parent);
    void setType(PortType type, PortPosition position);

};

