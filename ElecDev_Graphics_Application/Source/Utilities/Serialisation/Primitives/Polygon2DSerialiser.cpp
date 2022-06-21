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
	for (int i = 0; i < polygon2D->getVertexCount(); i++)
	{
		emitter << YAML::Key << "Vertex " + std::to_string(i) << YAML::Value << polygon2D->getVertex(i).position;
	}
	emitter << YAML::EndMap;
	// Color.
	emitter << YAML::Key << "Center" << YAML::Value << polygon2D->getTrackedCenter();
	emitter << YAML::Key << "Color" << YAML::Value << polygon2D->getColor();

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