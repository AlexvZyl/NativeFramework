#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Primitive.h"

//=============================================================================================================================================//
//  Forward decleration.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class GraphicsTrianglesBuffer;
class VertexDataCircle;

//=============================================================================================================================================//
//  Circle class.		 																													   //
//=============================================================================================================================================//

class Circle : public Primitive<GraphicsTrianglesBuffer<VertexDataCircle>>
{
public:

	// Local texture coordinates for the circle shader.
	static inline constexpr glm::vec2 local1 = { -1.f,  1.f };
	static inline constexpr glm::vec2 local2 = {  1.f,  1.f };
	static inline constexpr glm::vec2 local3 = {  1.f, -1.f };
	static inline constexpr glm::vec2 local4 = { -1.f, -1.f };

	// Circle data.
	float m_thickness = 0;
	float m_fade = 0;
	float m_radius = 0;

	// Constructors. 
	Circle(GraphicsTrianglesBuffer<VertexDataCircle>* gtb, const glm::vec3& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent);
	Circle(GraphicsTrianglesBuffer<VertexDataCircle>* gtb, const glm::vec2& center, float radius, const glm::vec4& color, float thickness, float fade, Entity* parent);
	void setRadius(float radius);
	void setThickness(float thickness);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
