#pragma once
#include "CoreGL/Entities/Entity.h"
#include "CoreGL/Entities/Primitive.h"

enum class PortType;
enum class PortPosition;

class Port;

template<typename VertexType>
class VertexArrayObject;

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

