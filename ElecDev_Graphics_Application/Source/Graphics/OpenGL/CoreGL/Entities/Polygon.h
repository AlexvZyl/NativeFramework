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

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template<typename VertexType>
class Polygon2D : public Primitive<VertexType>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Primitive<VertexType>::m_vertices;		
	using Primitive<VertexType>::m_indices;
	using Primitive<VertexType>::m_vertexCount;
	using Primitive<VertexType>::m_indexCount;
	using Primitive<VertexType>::m_VAO;		
	using Primitive<VertexType>::m_bufferStartIndex;			
	using Primitive<VertexType>::m_trackedCenter;
	using Primitive<VertexType>::m_colour;
	using Primitive<VertexType>::m_entityID;				

	// -------------------- //
	// P O L Y G O N   2 D  //
	// -------------------- //
	
	// Constructors.
	Polygon2D();
	Polygon2D(std::vector<glm::vec3> vertices, VertexArrayObject<VertexType>* VAO, ManagedEntity* parent);
	// Deconstructor
	~Polygon2D();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//