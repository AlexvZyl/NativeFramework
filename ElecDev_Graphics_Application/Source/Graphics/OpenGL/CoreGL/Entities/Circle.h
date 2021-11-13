#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Entity.h"

//=============================================================================================================================================//
//  Forward decleration.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;
class VertexDataCircle;

//=============================================================================================================================================//
//  Circle class.		 																													   //
//=============================================================================================================================================//

template <typename VertexType=VertexDataCircle>
class Circle : public Entity<VertexType>
{
private:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Entity<VertexType>::m_vertices;
	using Entity<VertexType>::m_VAO;
	using Entity<VertexType>::m_bufferStartIndex;
	using Entity<VertexType>::m_trackedCenter;
	using Entity<VertexType>::m_colour;
	using Entity<VertexType>::m_entityID;

	// ------------- //
	//  C I R C L E  //
	// ------------- //

	static inline glm::vec2 local1 = {-1.f, 1.f};
	static inline glm::vec2 local2 = {1.f, 1.f};
	static inline glm::vec2 local3 = {1.f, -1.f};
	static inline glm::vec2 local4 = {-1.f, -1.f};

	float m_thickness = 1.f;
	float m_fade = 0.001f;

public:

	// Constructors. 
	Circle();
	Circle(VertexArrayObject<VertexType>* vao, glm::vec3& center, float radius, glm::vec4& color, float thickness, float fade);
	// Destructor.
	~Circle();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
