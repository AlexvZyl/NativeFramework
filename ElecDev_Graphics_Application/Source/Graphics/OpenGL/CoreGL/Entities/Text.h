#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Entity.h"
#include <string>
#include <map>

//=============================================================================================================================================//
//  Forward declarations.																													   //
//=============================================================================================================================================//

template<typename VertexType>
class VertexArrayObject;
class VertexDataTextured;
struct Character;
struct Font;

//=============================================================================================================================================//
//  Class.																																	   //
//=============================================================================================================================================//

template<typename VertexType=VertexDataTextured>
class Text : public Entity<VertexType>
{
public:

	// ------------ //
	//  E N T I T Y //
	// ------------ //

	using Entity<VertexType>::m_vertices;
	using Entity<VertexType>::m_VAO;
	using Entity<VertexType>::m_bufferStartIndex;
	using Entity<VertexType>::m_trackedCenter;
	using Entity<VertexType>::m_colour;
	using Entity<VertexType>::m_entityID;

	// --------- //
	//  T E X T  //
	// --------- //

	// Constructor.
	Text(std::string text, glm::vec3* position, glm::vec4* color, float scale, 
		 VertexArrayObject<VertexType>* vao, Font* font, std::string align = "C");
	// Destructor.
	~Text();
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
