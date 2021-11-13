//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Vertex.h"
#include "Polygon.h"
#include "EntityManager.h"
#include "CoreGL/VertexArrayObjectGL.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																										       	   //
//=============================================================================================================================================//

template<typename VertexType>
Polygon2D<VertexType>::Polygon2D(){};

template<typename VertexType>
Polygon2D<VertexType>::Polygon2D(std::vector<glm::vec3>* vertices, VertexArrayObject<VertexType>* VAO)
{
	// General seyup.
	m_trackedCenter = glm::vec3(0.f, 0.f, 0.f);
	m_colour = glm::vec4(1.f, 0.f, 0.f, 0.5f);
	n_vertices = vertices->size();
	m_VAO = VAO;
	m_entityID = EntityManager::generateEID();

	// Populate VertexData structures.
	if (m_VAO->m_bufferType == GL_TRIANGLES) 
{
		//Tile the polygon in triangles sharing vertex 0.
		for (int i = 1; i < n_vertices - 1; i++) 
		{
			VertexData v1(&vertices->at(0), &m_colour, m_entityID);
			VertexData v2(&vertices->at(i), &m_colour, m_entityID);
			VertexData v3(&vertices->at(i + 1), &m_colour, m_entityID);
			m_vertices.insert(m_vertices.end(), { v1,v2,v3 });
		}
	}
	else if (m_VAO->m_bufferType == GL_LINES) 
	{
		// Add lines in a loop.
		for (int i = 0; i < n_vertices - 1; i++) 
		{
			VertexData v1(&vertices->at(i), &m_colour, m_entityID);
			VertexData v2(&vertices->at(i + 1), &m_colour, m_entityID);
			m_vertices.insert(m_vertices.end(), { v1,v2 });
		}
		// Close loop.
		VertexData v1(&vertices->at(n_vertices-1), &m_colour, m_entityID);
		VertexData v2(&vertices->at(0), &m_colour, m_entityID);
		m_vertices.insert(m_vertices.end(), { v1,v2 });
	}
	// Pass to VAO.
	n_vertices = m_vertices.size();
	m_VAO->appendDataCPU(this);
	m_VAO->updateGPU();
}

template<typename VertexType>
Polygon2D<VertexType>::~Polygon2D(){}

//=============================================================================================================================================//
//  Instantiations.																															   //
//=============================================================================================================================================//

template class Polygon2D<VertexData>;

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//
