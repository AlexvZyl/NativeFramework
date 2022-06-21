//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Primitives/PolyLine.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, PolyLine* polyline)
{
	// Begin text data.
	emitter << YAML::BeginMap;

	emitter << YAML::Key << "Closed" << YAML::Value << polyline->m_closed;
	emitter << YAML::Key << "Rounded" << YAML::Value << polyline->m_rounded;

	// Data.
	emitter << YAML::Key << "Vertices" << YAML::Value;
	// Vertices.
	emitter << YAML::BeginMap;
	int index = 0;
	for (auto& vertex : polyline->m_vertices)
	{
		emitter << YAML::Key << "Vertex " + std::to_string(index) << YAML::Value << vertex;
		index++;
	}
	emitter << YAML::EndMap;
	// Color.
	emitter << YAML::Key << "Center" << YAML::Value << polyline->getTrackedCenter();
	emitter << YAML::Key << "Color" << YAML::Value << polyline->getColor();
	emitter << YAML::Key << "Thickness" << YAML::Value << polyline->m_thickness;

	// End text data.
	emitter << YAML::EndMap;
	return emitter;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<PolyLine> polyline)
{
	return emitter << polyline.get();
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<PolyLine> polyline)
{
	return emitter << polyline.get();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//