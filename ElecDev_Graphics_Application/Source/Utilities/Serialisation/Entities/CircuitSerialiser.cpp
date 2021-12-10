//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Circuit.h"

//=============================================================================================================================================//
//  Serialise single circuit.  																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Circuit circuit) 
{
	// Circuit data.
	emitter << YAML::Key << "Circuit Info" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Label" << YAML::Value << circuit.m_label;
		emitter << YAML::Key << "Type" << YAML::Value << circuit.m_type;
	emitter << YAML::EndMap;
	// Components.
	emitter << YAML::Key << "Components" << YAML::Value << circuit.m_components;
	// Cables.
	emitter << YAML::Key << "Cables" << YAML::Value << circuit.m_cables;
	return emitter;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
