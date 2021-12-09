//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Component2D.h"
#include "CoreGL/FontLoader.h"

//=============================================================================================================================================//
//  Component 2D serialiser.   																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Component2D> comp) 
{
	// Begin component data.
	emitter << YAML::BeginMap;

	// Component data.
	emitter << YAML::Key << "Centre" << YAML::Value << comp->centre;
	emitter << YAML::Key << "Component Layer" << YAML::Value << comp->componentLayer;
	emitter << YAML::Key << "Border layer offset" << YAML::Value << comp->borderLayerOffset;
	emitter << YAML::Key << "Layer offset" << YAML::Value << comp->portLayerOffset;
	emitter << YAML::Key << "Number of ports" << YAML::Value << comp->numPorts;
	emitter << YAML::Key << "Internal circuit" << YAML::Value << "Test AE 234";

	// Shape.
	emitter << YAML::Key << "Shape" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Colour" << YAML::Value << comp->shapeColour;
	emitter << YAML::EndMap;

	// Title.
	emitter << YAML::Key << "Title" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Font" << YAML::Value << comp->titleFont.name;
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

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>> compVector) 
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

void deserialise(YAML::Node yamlComp, std::shared_ptr<Component2D> component) 
{
	component->selected = false;
	component->unhighlight();
	// Move component.
	float position[2] = {
		yamlComp["Centre"][0].as<float>(),
		yamlComp["Centre"][1].as<float>()
	};
	component->moveTo(position);

	// Set shape colour.
	YAML::Node shape = yamlComp["Shape"];
	glm::vec4 shapeColour(
		shape["Colour"][0].as<float>(),
		shape["Colour"][1].as<float>(),
		shape["Colour"][2].as<float>(),
		shape["Colour"][3].as<float>()
	);
	component->shapeColour = shapeColour;
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
