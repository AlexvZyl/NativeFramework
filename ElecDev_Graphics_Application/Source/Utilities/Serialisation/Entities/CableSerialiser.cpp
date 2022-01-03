//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Cable.h"
#include "CoreGL/Entities/Circle.h"

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
	emitter << YAML::Key << "Nodes" << YAML::Value;
	emitter << YAML::BeginMap;
	int nodeIndex = 0;
	for (std::shared_ptr<Circle> circle : cable->m_nodes)
	{
		emitter << YAML::Key << "Node " + std::to_string(nodeIndex) << YAML::Value << circle->m_trackedCenter;
		nodeIndex++;
	}
	emitter << YAML::EndMap;

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
//  Single Cable deserialiser.																												   //
//=============================================================================================================================================//

void deserialise(YAML::Node& yamlNode, std::shared_ptr<Cable> cable)
{

}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//