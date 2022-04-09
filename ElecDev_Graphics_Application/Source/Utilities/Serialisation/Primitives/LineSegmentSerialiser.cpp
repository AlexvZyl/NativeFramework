#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Primitives/LineSegment.h"

YAML::Emitter& operator<<(YAML::Emitter& emitter, LineSegment* lineSegment) 
{
	// Begin text data.
	emitter << YAML::BeginMap;

	// Data.
	emitter << YAML::Key << "Start" << YAML::Value << lineSegment->m_start;
	emitter << YAML::Key << "End" << YAML::Value << lineSegment->m_end;
	emitter << YAML::Key << "Color" << YAML::Value << lineSegment->m_colour;
	emitter << YAML::Key << "Thickness" << YAML::Value << lineSegment->m_thickness;

	// End text data.
	emitter << YAML::EndMap;
	return emitter;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<LineSegment> lineSegment) 
{
	return emitter << lineSegment.get();
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<LineSegment> lineSegment) 
{
	return emitter << lineSegment.get();
}