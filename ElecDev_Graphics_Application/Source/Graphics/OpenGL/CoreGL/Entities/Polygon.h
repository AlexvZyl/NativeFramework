#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "CoreGL/Entities/Entity.h"
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
class Polygon2D : public Entity<VertexType>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Entity<VertexType>::m_vertices;		
	using Entity<VertexType>::m_indices;
	using Entity<VertexType>::m_vertexCount;
	using Entity<VertexType>::m_indexCount;
	using Entity<VertexType>::m_VAO;		
	using Entity<VertexType>::m_bufferStartIndex;			
	using Entity<VertexType>::m_trackedCenter;
	using Entity<VertexType>::m_colour;
	using Entity<VertexType>::m_entityID;				

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