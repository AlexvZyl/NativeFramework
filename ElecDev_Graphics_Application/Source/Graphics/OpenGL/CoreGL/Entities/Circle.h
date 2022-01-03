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

class Circle : public Primitive<VertexDataCircle>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Primitive<VertexDataCircle>::m_vertices;
	using Primitive<VertexDataCircle>::m_indices;
	using Primitive<VertexDataCircle>::m_vertexCount;
	using Primitive<VertexDataCircle>::m_indexCount;
	using Primitive<VertexDataCircle>::m_VAO;
	using Primitive<VertexDataCircle>::m_bufferStartIndex;
	using Primitive<VertexDataCircle>::m_trackedCenter;
	using Primitive<VertexDataCircle>::m_colour;
	using Primitive<VertexDataCircle>::m_entityID;

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
	Circle(VertexArrayObject<VertexDataCircle>* vao, glm::vec3 center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent);
	Circle(VertexArrayObject<VertexDataCircle>* vao, glm::vec2 center, float radius, glm::vec4& color, float thickness, float fade, Entity* parent);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
