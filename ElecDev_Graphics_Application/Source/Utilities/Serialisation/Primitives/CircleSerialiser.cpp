//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Utilities/Serialisation/Serialiser.h"
#include "OpenGL/Primitives/Circle.h"

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Circle* circle) 
{
	// Begin text data.
	emitter << YAML::BeginMap;

	// Data.
	emitter << YAML::Key << "Center" << YAML::Value << circle->getTrackedCenter();
	emitter << YAML::Key << "Radius" << YAML::Value << circle->m_radius;
	emitter << YAML::Key << "Fade" << YAML::Value << circle->m_fade;
	emitter << YAML::Key << "Thickness" << YAML::Value << circle->m_thickness;
	emitter << YAML::Key << "Color" << YAML::Value << circle->getColor();

	// End text data.
	emitter << YAML::EndMap;
	return emitter;
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::unique_ptr<Circle> circle)
{
	return emitter << circle.get();
}

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Circle> circle)
{
	return emitter << circle.get();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//s