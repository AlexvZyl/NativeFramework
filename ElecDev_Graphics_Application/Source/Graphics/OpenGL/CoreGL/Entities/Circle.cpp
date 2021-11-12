//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Circle.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

template<typename VertexType>
Circle<VertexType>::Circle(const VertexArrayObject<VertexType>* vao, const glm::vec3* center, float radius, const glm::vec4* color)
{
	//m_VAO = vao;
	//m_trackedCenter = *center;
	//m_colour = *color;
	//m_entityID = 0;

	//Vertex* v1 = new VertexDataCircle(
	//	glm::vec3(),
	//	m_colour,
	//	glm::vec2(),
	//	m_entityID
	//);
	//Vertex* v2= new VertexDataCircle(
	//	glm::vec3(),
	//	m_colour,
	//	glm::vec2(),
	//	m_entityID
	//);
	//Vertex* v3 = new VertexDataCircle(
	//	glm::vec3(),
	//	m_colour,
	//	glm::vec2(),
	//	m_entityID
	//);
	//Vertex* v4 = new VertexDataCircle(
	//	glm::vec3(),
	//	m_colour,
	//	glm::vec2(),
	//	m_entityID
	//);

	//m_vertices = {v1,v2,v3,v3,v4,v1};
	//m_VAO->appendDataCPU(this);
}

template<typename VertexType>
Circle<VertexType>::~Circle() {}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//