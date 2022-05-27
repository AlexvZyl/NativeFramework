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

template<typename VertexType, typename IndexType>
class VertexArrayObject;

class VertexData;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

class Polygon2D : public Primitive<VertexData, IndexData3>
{
public:				
	
	// Constructors.
	Polygon2D(const std::vector<glm::vec3>& vertices, VertexArrayObject<VertexData, IndexData3>* VAO, Entity* parent, glm::vec4 colour = {0.f, 0.f, 0.f, 1.f});

	// Adds a new vertex and handles the required manipulation of data.
	virtual void pushVertex(const glm::vec3& vertex);
	virtual void translateVertexAtIndex(unsigned index, const glm::vec3& translation) override;
	virtual void translateVertexAtIndex(unsigned index, const glm::vec2& translation) override;
	// Translate to.
	virtual void translateToVertexAtIndex(unsigned index, const glm::vec3& position) override;
	virtual void translateToVertexAtIndex(unsigned index, const glm::vec2& position) override;
	virtual void updateIndices();
	// Move a vertex
	virtual void translateVertexTo(VertexData* vertex, const glm::vec3 position) override;
	// Move a vertex
	virtual void translateVertexTo(VertexData* vertex, const glm::vec2 position) override;
	// Move a vertex
	virtual void translateVertex(VertexData* vertex, const glm::vec3 translation) override;
	// Move a vertex
	virtual void translateVertex(VertexData* vertex, const glm::vec2 translation) override;
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//