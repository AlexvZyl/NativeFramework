//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Primitives/Polygon.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Polygon2D* polygon2D) 
{
	// Begin text data.
	emitter << YAML::BeginMap;

	// Data.
	emitter << YAML::Key << "Vertices" << YAML::Value;
	// Vertices.
	emitter << YAML::BeginMap;
	int index = 0;
	VertexArrayObject<VertexData>* vao = polygon2D->m_VAO;
	for (int i = polygon2D->m_vertexBufferPos; i < polygon2D->m_vertexCount + polygon2D->m_vertexBufferPos; i++)
	{
		emitter << YAML::Key << "Vertex " + std::to_string(index) << YAML::Value << vao->m_vertexData[i].data.position;
		index++;
	}
	emitter << YAML::EndMap;
	// Color.
	emitter << YAML::Key << "Center" << YAML::Value << polygon2D->m_trackedCenter;
	emitter << YAML::Key << "Color" << YAML::Value << polygon2D->m_colour;

	// End text data.
	emitter << YAML::EndMap;
	return emitter;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<Polygon2D> polygon2D) 
{
	return emitter << polygon2D.get();
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Polygon2D> polygon2D) 
{
	return emitter << polygon2D.get();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//