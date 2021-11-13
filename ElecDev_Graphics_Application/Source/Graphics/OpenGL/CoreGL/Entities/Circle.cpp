//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Circle.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Entities/EntityManager.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

template<typename VertexType>
Circle<VertexType>::Circle(VertexArrayObject<VertexType>* vao, glm::vec3& center, float radius, glm::vec4& color, float thickness, float fade)
{
	// Assign variables.
	m_VAO = vao;
	m_trackedCenter = center;
	m_colour = color;
	m_entityID = EntityManager::generateEID();
	m_thickness = thickness;
	m_fade = fade;

	glm::vec3 pos1(center.x - radius, center.y + radius, center.z);
	glm::vec3 pos2(center.x + radius, center.y + radius, center.z);
	glm::vec3 pos3(center.x + radius, center.y - radius, center.z);
	glm::vec3 pos4(center.x - radius, center.y - radius, center.z);
	VertexDataCircle v1(pos1, local1, color, m_thickness, m_fade, m_entityID);
	VertexDataCircle v2(pos2, local2, color, m_thickness, m_fade, m_entityID);
	VertexDataCircle v3(pos3, local3, color, m_thickness, m_fade, m_entityID);
	VertexDataCircle v4(pos4, local4, color, m_thickness, m_fade, m_entityID);
	m_vertices.insert(m_vertices.end(), { v1,v2,v3,v3,v4,v1 });
	m_VAO->appendDataCPU(this);
}

template<typename VertexType>
Circle<VertexType>::~Circle() {}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Circle<VertexDataCircle>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//