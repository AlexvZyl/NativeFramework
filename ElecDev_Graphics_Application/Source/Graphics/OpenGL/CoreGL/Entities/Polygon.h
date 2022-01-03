#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/Entities/Primitive.h"
#include <vector>
#include <glm.hpp>

//=============================================================================================================================================//
//  Forward dclerations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;

class VertexData;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Polygon2D : public Primitive<VertexData>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Primitive<VertexData>::m_vertices;		
	using Primitive<VertexData>::m_indices;
	using Primitive<VertexData>::m_vertexCount;
	using Primitive<VertexData>::m_indexCount;
	using Primitive<VertexData>::m_VAO;		
	using Primitive<VertexData>::m_bufferStartIndex;			
	using Primitive<VertexData>::m_trackedCenter;
	using Primitive<VertexData>::m_colour;
	using Primitive<VertexData>::m_entityID;				

	// -------------------- //
	// P O L Y G O N   2 D  //
	// -------------------- //
	
	// Constructors.
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject<VertexData>* VAO, Entity* parent);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//