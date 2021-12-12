//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "CoreGL/Entities/LineSegment.h"

//=============================================================================================================================================//
//  Cable serialiser.     																													   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<LineSegment> lineSegment)
{
	// Begin line segment data.
	emitter << YAML::BeginMap;

	emitter << YAML::Key << "Entity ID" << YAML::Value << lineSegment->m_entityID;
	emitter << YAML::Key << "Start" << YAML::Value << lineSegment->m_start;
	emitter << YAML::Key << "End" << YAML::Value << lineSegment->m_end;
	emitter << YAML::Key << "Thickness" << YAML::Value << lineSegment->m_thickness;

	// End line segment data.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Cable vector serialiser.																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<LineSegment>> lineSegmentVector)
{
	emitter << YAML::BeginMap;
	int lineIndex = 0;
	for (std::shared_ptr<LineSegment> lineSegment : lineSegmentVector)
	{
		// Begin cable.
		emitter << YAML::Key << "Line Segment " + std::to_string(lineIndex) << YAML::Value << lineSegment;
		lineIndex++;
	}
	// End cables.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//