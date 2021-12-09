//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Port.h"
#include "CoreGL/FontLoader.h"

//=============================================================================================================================================//
//  Single port.		     																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Port> port)
{
	// Begin the port Map.
	emitter << YAML::BeginMap;

	// Port data.
	emitter << YAML::Key << "Label" << YAML::Value << port->m_label;
	emitter << YAML::Key << "Centre" << YAML::Value << port->centre;
	emitter << YAML::Key << "Layer" << YAML::Value << port->portLayer;
	emitter << YAML::Key << "Offset" << YAML::Value << port->m_offset;
	emitter << YAML::Key << "Type" << YAML::Value << port->m_type;

	// Border.
	emitter << YAML::Key << "Border" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Thickness" << YAML::Value << port->border.m_thickness;
		emitter << YAML::Key << "Fade" << YAML::Value << port->border.m_fade;
		emitter << YAML::Key << "Colour" << YAML::Value << port->borderColour;
	emitter << YAML::EndMap;

	// Body.
	emitter << YAML::Key << "Body" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Thickness" << YAML::Value << port->body.m_thickness;
		emitter << YAML::Key << "Fade" << YAML::Value << port->body.m_fade;
		emitter << YAML::Key << "Colour" << YAML::Value << port->bodyColour;
	emitter << YAML::EndMap;

	// Title.
	emitter << YAML::Key << "Title" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Font" << YAML::Value << port->titleFont.name;
		emitter << YAML::Key << "Size" << YAML::Value << port->titleSize;
		emitter << YAML::Key << "Offset" << YAML::Value << port->titleOffset;
		emitter << YAML::Key << "Colour" << YAML::Value << port->titleColour;
	emitter << YAML::EndMap;

	// End the port map.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  Port vector.		     																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Port>> portVector)
{
	// Begin.
	emitter << YAML::BeginMap;

	int portIndex = 0;
	// Iterate through all of the ports.
	for (std::shared_ptr<Port> port : portVector)
	{
		emitter << YAML::Key << "Port " + std::to_string(portIndex) << YAML::Value << port;
		portIndex++;
	}
	
	// Done.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  EOF.				    																												   //
//=============================================================================================================================================//

