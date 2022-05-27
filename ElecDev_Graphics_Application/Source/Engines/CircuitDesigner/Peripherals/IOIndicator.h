#pragma once

//==============================================================================================================================================//
//  Includes.																															        //
//==============================================================================================================================================//

#include "OpenGL/Primitives/Primitive.h"

//==============================================================================================================================================//
//  Forward Declerations.																													    //
//==============================================================================================================================================//

enum class PortType;
enum class PortPosition;

class Port;
class VertexData;

template<typename VertexType, typename IndexType>
class VertexArrayObject;

//==============================================================================================================================================//
//  IOIndicator Class.																															//
//==============================================================================================================================================//

class IOIndicator : public Primitive<VertexData>
{

private:

    // Data.
    PortType m_portType;
    PortPosition m_portPosition;

public:

    // Comstructor.
    IOIndicator(PortType type, PortPosition position, VertexArrayObject<VertexData, IndexData3>* VAO, Port* parent);
    // Set the IO type.
    void setType(PortType type, PortPosition position);

};

//==============================================================================================================================================//
//  EOF.																															            //
//==============================================================================================================================================//