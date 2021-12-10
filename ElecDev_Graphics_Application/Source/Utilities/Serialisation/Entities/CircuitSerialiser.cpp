//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Circuit.h"
#include "Graphics/OpenGL/Design2DEngineGL/Design2D_Engine.h"

//=============================================================================================================================================//
//  Serialise single circuit.  																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Circuit> circuit) 
{
	// Circuit data.
	emitter << YAML::Key << "Circuit Info" << YAML::Value;
	emitter << YAML::BeginMap;
		emitter << YAML::Key << "Label" << YAML::Value << circuit->m_label;
		emitter << YAML::Key << "Type" << YAML::Value << circuit->m_type;
	emitter << YAML::EndMap;
	// Components.
	emitter << YAML::Key << "Components" << YAML::Value << circuit->m_components;
	// Cables.
	emitter << YAML::Key << "Cables" << YAML::Value << circuit->m_cables;
	return emitter;
}

//=============================================================================================================================================//
//  Serialise circuit vector.  																												   //
//=============================================================================================================================================//

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Circuit>> circuitVector) 
{
	// Begin.
	emitter << YAML::BeginMap;
	int circuitIndex = 0;
	for (std::shared_ptr<Circuit> circuit : circuitVector)
	{
		// Begin component.
		emitter << YAML::Key << "Circuit " + std::to_string(circuitIndex) << YAML::Value << circuit;
		circuitIndex++;
	}
	// Done.
	emitter << YAML::EndMap;
	return emitter;
}

//=============================================================================================================================================//
//  Circuit loading.		  																												   //
//=============================================================================================================================================//

void loadCircuitFromYAML(std::string path, Design2DEngineGL* engine)
{
	// Create yaml node from file.
	YAML::Node yamlFile = YAML::LoadFile(path);

	// --------------- //
	//  C I R C U I T  //
	// --------------- //

	YAML::Node circuitInfo = yamlFile["Circuit Info"];
	engine->m_circuit->m_label = circuitInfo["Label"].as<std::string>();
	engine->m_circuit->m_type = circuitInfo["Type"].as<std::string>();

	// -------------------- //
	// C O M P O N E N T S  //
	// -------------------- //

	// Load all of the components.
	YAML::Node componentList = yamlFile["Components"];
	// Iterate through all of the components.
	for (YAML::iterator comp = componentList.begin(); comp != componentList.end(); ++comp)
	{
		// Create component.
		std::shared_ptr<Component2D> component = std::make_shared<Component2D>(engine->m_trianglesVAO.get(),
																			   engine->m_linesVAO.get(),
																			   engine->m_texturedTrianglesVAO.get(),
																			   engine->m_circlesVAO.get(),
																			   engine->m_circuit.get());
		// Load data into component.
		deserialise(comp->second, component);	// The second part of the iterator is the YAML Node.
		// Push onto componenets vector.
		engine->m_circuit->m_components.push_back(component);
	}

	// ------------- //
	//  C A B L E S  //
	// ------------- //
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
