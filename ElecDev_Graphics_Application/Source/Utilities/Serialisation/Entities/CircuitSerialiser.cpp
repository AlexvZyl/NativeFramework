//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Port.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Graphics/Entities/EntityManager.h"
#include "Lumen.h"
#include "Application/Application.h"

//=============================================================================================================================================//
//  Function declerations.			     																									   //
//=============================================================================================================================================//

// Return the type of the port.
PortType getPortType(YAML::Node node);

//=============================================================================================================================================//
//  Serialise single circuit.  																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, Circuit* circuit) 
{
	// Circuit data.
	emitter << YAML::Key << "Circuit Info" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Label" << YAML::Value << circuit->m_label;
		emitter << YAML::Key << "Type" << YAML::Value << circuit->m_type;
	emitter << YAML::EndMap;

	// Store references to components.
	emitter << YAML::Key << "Components" << YAML::Value;
	emitter << YAML::BeginMap;
	int index = 0;
	for (auto& comp : circuit->m_components)
	{
		emitter << YAML::Key << "Component " + std::to_string(index) << YAML::Value;
		emitter << YAML::BeginMap;
			emitter << YAML::Key << "Filename" << YAML::Value <<  comp->equipType + ".lmcp";
			emitter << YAML::Key << "Designator Index" << YAML::Value << comp->designatorIdx;
			emitter << YAML::Key << "Position" << YAML::Value << comp->centre;
			emitter << YAML::Key << "Rotation" << YAML::Value << comp->m_rotation;
			emitter << YAML::Key << "Dictionary" << YAML::Value << comp->dataDict;
			emitter << YAML::Key << "Tag" << YAML::Value << comp->m_toTagNumber;
			emitter << YAML::Key << "From Tag" << YAML::Value << comp->m_fromTagNumber;
		emitter << YAML::EndMap;
		index++;
	}
	emitter << YAML::EndMap;

	// Cables.
	emitter << YAML::Key << "Cables" << YAML::Value;
	emitter << YAML::BeginMap;
	int cablesIndex = 0;
	for (auto& cable : circuit->m_cables)
	{
		emitter << YAML::Key << "Cable " + std::to_string(cablesIndex) << YAML::Value;
		serialiseCable(emitter, cable.get(), circuit);
		cablesIndex++;
	}
	// End cables.
	emitter << YAML::EndMap;

	// Reference components.
	emitter << YAML::Key << "Reference Components" << YAML::Value;
	emitter << YAML::BeginMap;
	for (auto& [name, node] : circuit->m_referenceComponents)
	{
		emitter << YAML::Key << name << YAML::Value << node;
	}
	// End reference components.
	emitter << YAML::EndMap;

	// Reference cables.
	emitter << YAML::Key << "Reference Cables" << YAML::Value;
	emitter << YAML::BeginMap;
	for (auto& [name, node] : circuit->m_referenceCables)
	{
		emitter << YAML::Key << name << YAML::Value << node;
	}
	// End reference cables.
	emitter << YAML::EndMap;

	return emitter;
}

//=============================================================================================================================================//
//  Serialise circuit vector.  																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Circuit>>& circuitVector) 
{
	// Begin.
	emitter << YAML::BeginMap;
	int circuitIndex = 0;
	for (std::shared_ptr<Circuit> circuit : circuitVector)
	{
		// Begin component.
		emitter << YAML::Key << "Circuit " + std::to_string(circuitIndex) << YAML::Value << circuit.get();
		circuitIndex++;
	}
	// Done.
	emitter << YAML::EndMap;
	return emitter;
}


//=============================================================================================================================================//
//  Utilities.				     																											   //
//=============================================================================================================================================//

PortType getPortType(YAML::Node node) 
{
	if		(node["Type"].as<std::string>() == "PORT_IN")	 { return PortType::PORT_IN;    }
	else if (node["Type"].as<std::string>() == "PORT_OUT")	 { return PortType::PORT_OUT;   }
	else if (node["Type"].as<std::string>() == "PORT_INOUT") { return PortType::PORT_INOUT; }           
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
