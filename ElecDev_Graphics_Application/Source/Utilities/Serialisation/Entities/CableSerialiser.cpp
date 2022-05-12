//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "OpenGL/Primitives/Circle.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"

//=============================================================================================================================================//
//  Cable serialiser.     																													   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Cable* cable) 
{
	// Store the cable type, if one is supplied.
	std::string filename = "";
	if (cable->m_cableType.size())	filename = (cable->m_cableType + ".lmcb");
	emitter << YAML::Key << "Filename" << YAML::Value << filename;

	// General data.
	emitter << YAML::Key << "Thickness" << YAML::Value << cable->m_thickness;

	// Data dictionary.
	emitter << YAML::Key << "Dictionary" << YAML::Value << cable->cableDict;

	// Nodes.
	emitter << YAML::Key << "Nodes" << YAML::Value;
	emitter << YAML::BeginMap;
	int nodeIndex = 0;
	for (glm::vec2 node : cable->m_polyLine->m_vertices)
	{
		emitter << YAML::Key << "Node " + std::to_string(nodeIndex) << YAML::Value << node;
		nodeIndex++;
	}
	emitter << YAML::EndMap;

	return emitter;
}

void serialiseCable(YAML::Emitter& emitter, Cable* cable, Circuit* circuit)
{
	// Begin the cable data.
	emitter << YAML::BeginMap;

	//  Cable general data.
	emitter << cable;

	// Find the start port indices.
	int startComponentIndex = 0;
	int startPortIndex = 0;
	bool found = false;
	for (auto& component : circuit->m_components)
	{
		for (auto& port : component->ports)
		{
			if (port.get() == cable->m_startPort)
			{
				found = true;
				break;
			}
			startPortIndex++;
		}
		if (found)
			break;
		startPortIndex = 0;
		startComponentIndex++;
	}

	// Store the indices.
	emitter << YAML::Key << "Start Component Index" << YAML::Value << startComponentIndex;
	emitter << YAML::Key << "Start Port Index" << YAML::Value << startPortIndex;

	// Find the end port indices.
	int endComponentIndex = 0;
	int endPortIndex = 0;
	found = false;
	for (auto& component : circuit->m_components)
	{
		for (auto& port : component->ports)
		{
			if (port.get() == cable->m_endPort)
			{
				found = true;
				break;
			}
			endPortIndex++;
		}
		if (found)
			break;
		endPortIndex = 0;
		endComponentIndex++;
	}

	// Store the indices.
	emitter << YAML::Key << "End Component Index" << YAML::Value << endComponentIndex;
	emitter << YAML::Key << "End Port Index" << YAML::Value << endPortIndex;

	// End the port map.
	emitter << YAML::EndMap;
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
		emitter << YAML::Key << "Cable " + std::to_string(cablesIndex) << YAML::Value << cable.get();
		cablesIndex++;
	}
	// End cables.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//