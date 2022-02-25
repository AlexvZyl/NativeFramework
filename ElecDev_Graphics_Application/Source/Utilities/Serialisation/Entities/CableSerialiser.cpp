//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "OpenGL/Primitives/Circle.h"

//=============================================================================================================================================//
//  Cable serialiser.     																													   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Cable>& cable) 
{
	// Begin cable data.
	emitter << YAML::BeginMap;

	// General data.
	emitter << YAML::Key << "Entity ID" << YAML::Value << cable->m_entityID;
	emitter << YAML::Key << "Thickness" << YAML::Value << cable->m_thickness;
	emitter << YAML::Key << "Start port" << cable->m_startPort->m_entityID;
	emitter << YAML::Key << "End port" << cable->m_endPort->m_entityID;
	//emitter << YAML::Key << "Title1 pos" << YAML::Value << cable->m_title1->m_trackedCenter;
	//emitter << YAML::Key << "Title2 pos" << YAML::Value << cable->m_title2->m_trackedCenter;
	emitter << YAML::Key << "Title" << YAML::Value << cable->m_titleString;
	emitter << YAML::Key << "Dictionary" << YAML::Value << cable->cableDict;

	// Nodes.
	emitter << YAML::Key << "Nodes" << YAML::Value;
	emitter << YAML::BeginMap;
	int nodeIndex = 0;
	for (Circle* circle : cable->m_nodes)
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

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Cable>>& cableVector)
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

void deserialise(YAML::Node& yamlNode, std::shared_ptr<Cable>& cable)
{

}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//