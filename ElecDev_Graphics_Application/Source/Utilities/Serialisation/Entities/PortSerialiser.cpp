//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"
#include "Graphics/Fonts/FontLoader.h"

//=============================================================================================================================================//
//  Single port.		     																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Port* port)
{
	// Begin the port Map.
	emitter << YAML::BeginMap;

	// Port data.
	emitter << YAML::Key << "Label" << YAML::Value << port->m_label;
	emitter << YAML::Key << "Entity ID" << YAML::Value << port->m_entityID;
	emitter << YAML::Key << "Centre" << YAML::Value << port->centre;
	emitter << YAML::Key << "Layer" << YAML::Value << port->portLayer;
	emitter << YAML::Key << "Type" << YAML::Value << port->m_type;

	// Border.
	emitter << YAML::Key << "Border" << YAML::Value << port->border;

	// Body.
	emitter << YAML::Key << "Body" << YAML::Value << port->body;

	// Title.
	emitter << YAML::Key << "Title" << YAML::Value << port->title;

	// End the port map.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  Port vector.		     																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Port>>& portVector)
{
	// Begin.
	emitter << YAML::BeginMap;

	int portIndex = 0;
	// Iterate through all of the ports.
	for (std::shared_ptr<Port> port : portVector)
	{
		emitter << YAML::Key << "Port " + std::to_string(portIndex) << YAML::Value << port.get();
		portIndex++;
	}
	
	// Done.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  Utilities.																																   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type)
{
	std::string typeStr;
	if		(type == PortType::PORT_IN)		typeStr = "PORT_IN";
	else if (type == PortType::PORT_OUT)	typeStr = "PORT_OUT";
	else if (type == PortType::PORT_INOUT)	typeStr = "PORT_INOUT";
	return emitter << typeStr;
}

//=============================================================================================================================================//
//  EOF.				    																												   //
//=============================================================================================================================================//

