#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Primitive.h"

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
class Circle : public Primitive<VertexType>
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

	// ------------- //
	//  C I R C L E  //
	// ------------- //

	static inline glm::vec2 local1 = {-1.f, 1.f};
	static inline glm::vec2 local2 = {1.f, 1.f};
	static inline glm::vec2 local3 = {1.f, -1.f};
	static inline glm::vec2 local4 = {-1.f, -1.f};

	float m_thickness = 0;
	float m_fade = 0;

	// Constructors. 
	Circle() = default;
	Circle(VertexArrayObject<VertexType>* vao, glm::vec3 center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent);
	Circle(VertexArrayObject<VertexType>* vao, glm::vec2 center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent);
	// Destructor.
	~Circle();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
