#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <vector>
#include <glm/glm.hpp>
#include "OpenGL/Primitives/Primitive.h"
#include <earcut/include/mapbox/earcut.hpp>


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
	
	// Constructors.
	Polygon2D(const std::vector<glm::vec3>& vertices, VertexArrayObject<VertexData>* VAO, Entity* parent);

	// Adds a new vertex and handles the required manipulation of data.
	void pushVertex(const glm::vec3& vertex);
	void translateVertexAtIndex(unsigned index, const glm::vec3& translation) override;
	void translateVertexAtIndex(unsigned index, const glm::vec2& translation) override;
	// Translate to.
	void translateToVertexAtIndex(unsigned index, const glm::vec3& position) override;
	void translateToVertexAtIndex(unsigned index, const glm::vec2& position) override;
	void updateIndices();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//