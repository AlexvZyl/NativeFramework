//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Cable.h"

//=============================================================================================================================================//
//  Cable serialiser.     																													   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Cable> cable) 
{
	// Begin cable data.
	emitter << YAML::BeginMap;

	// General data.
	emitter << YAML::Key << "Entity ID" << YAML::Value << cable->m_entityID;
	emitter << YAML::Key << "Thickness" << YAML::Value << cable->m_thickness;
	emitter << YAML::Key << "Start port" << cable->m_startPort->m_entityID;
	emitter << YAML::Key << "End port" << cable->m_endPort->m_entityID;

	// Nodes.
	emitter << YAML::Key << "Line Segments" << YAML::Value << cable->m_lines;

	// End cable data.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Cable vector serialiser.																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Cable>> cableVector)
{
	emitter << YAML::BeginMap;
	int cablesIndex = 0;
	for (std::shared_ptr<Cable> cable : cableVector)
	{
		// Begin cable.
		emitter << YAML::Key << "Cable " + std::to_string(cablesIndex) << YAML::Value << cable;
		cablesIndex++;
	}
	// End cables.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//