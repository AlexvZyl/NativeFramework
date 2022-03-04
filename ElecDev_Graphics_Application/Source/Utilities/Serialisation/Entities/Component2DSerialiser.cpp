//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Graphics/Fonts/FontLoader.h"

//=============================================================================================================================================//
//  Component 2D serialiser.   																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Component2D>& comp) 
{
	// Begin component data.
	emitter << YAML::BeginMap;

	// Component data.
	emitter << YAML::Key << "Centre" << YAML::Value << comp->centre;
	emitter << YAML::Key << "Entity ID" << YAML::Value << comp->m_entityID;
	emitter << YAML::Key << "Component Layer" << YAML::Value << comp->componentLayer;
	emitter << YAML::Key << "Border layer offset" << YAML::Value << comp->borderLayerOffset;
	emitter << YAML::Key << "Layer offset" << YAML::Value << comp->portLayerOffset;
	emitter << YAML::Key << "Number of ports" << YAML::Value << comp->numPorts;
	emitter << YAML::Key << "Internal circuit" << YAML::Value << "Test AE 234";
	emitter << YAML::Key << "Dictionary" << YAML::Value << comp->dataDict;

	// Shape.
	emitter << YAML::Key << "Shape" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Colour" << YAML::Value << comp->shapeColour;
	emitter << YAML::EndMap;

	// Title.
	emitter << YAML::Key << "Title" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "String" << YAML::Value << comp->titleString;
		emitter << YAML::Key << "Size" << YAML::Value << comp->titleSize;
		emitter << YAML::Key << "Offset" << YAML::Value << comp->titleOffset;
		emitter << YAML::Key << "Colour" << YAML::Value << comp->titleColour;
	emitter << YAML::EndMap;

	// Ports.
	emitter << YAML::Key << "Ports" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "East Ports" << YAML::Value << comp->portsEast;
		emitter << YAML::Key << "West Ports" << YAML::Value << comp->portsWest;
		emitter << YAML::Key << "North Ports" << YAML::Value << comp->portsNorth;
		emitter << YAML::Key << "South Ports" << YAML::Value << comp->portsSouth;
	emitter << YAML::EndMap;

	// End componenet data.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Component2D Vector serialiser.   																										   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>>& compVector) 
{
	// Begin.
	emitter << YAML::BeginMap;
	int componentIndex = 0;
	for (std::shared_ptr<Component2D> comp : compVector)
	{
		// Begin component.
		emitter << YAML::Key << "Component " + std::to_string(componentIndex) << YAML::Value << comp;
		componentIndex++;
	}
	// Done.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Deserialise single Component2D  																										   //
//=============================================================================================================================================//

void deserialise(YAML::Node& yamlNode, std::shared_ptr<Component2D>& component)
{

}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
