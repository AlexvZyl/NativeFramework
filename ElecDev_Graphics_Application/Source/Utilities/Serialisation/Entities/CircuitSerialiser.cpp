//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Circuit.h"
#include "Graphics/OpenGL/Design2DEngineGL/Design2D_Engine.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Port.h"

//=============================================================================================================================================//
//  Function declerations.			     																									   //
//=============================================================================================================================================//

PortType getPortType(YAML::Node node);

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
//  Single Circuit deserialiser.																											   //
//=============================================================================================================================================//

void deserialise(YAML::Node& yamlNode, std::shared_ptr<Circuit> circuit, Design2DEngineGL& engine)
{
	// --------------- //
	//  C I R C U I T  //
	// --------------- //

	YAML::Node circuitInfo = yamlNode["Circuit Info"];
	engine.m_circuit.reset();
	engine.m_circuit = std::make_unique<Circuit>(
		circuitInfo["Label"].as<std::string>(),
		circuitInfo["Type"].as<std::string>()
		);

	// -------------------- //
	// C O M P O N E N T S  //
	// -------------------- //

	// Load all of the components.
	YAML::Node componentList = yamlNode["Components"];
	// Iterate through all of the components.
	for (YAML::iterator compIterator = componentList.begin(); compIterator != componentList.end(); ++compIterator)
	{
		YAML::Node componentNode = compIterator->second;
		// Create component.
		std::shared_ptr<Component2D> component = std::make_shared<Component2D>(engine.m_trianglesVAO.get(),
																			   engine.m_linesVAO.get(),
																			   engine.m_texturedTrianglesVAO.get(),
																			   engine.m_circlesVAO.get(),
																			   engine.m_circuit.get());
		// Load data into component.
		float position[2] = {
		yamlNode["Centre"][0].as<float>(),
		yamlNode["Centre"][1].as<float>()
		};
		component->place(position);

		// ----------- //
		//  P O R T S  //
		// ----------- //

		YAML::Node portList = componentNode["Ports"];

		// East ports.
		YAML::Node eastPortList = portList["East Ports"];
		for (YAML::iterator portIterator = eastPortList.begin(); portIterator != eastPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			component->addPort(1, getPortType(portNode), portNode["Label"].as<std::string>());
		}
		// West ports.
		YAML::Node westPortList = portList["West Ports"];
		for (YAML::iterator portIterator = westPortList.begin(); portIterator != westPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			component->addPort(0, getPortType(portNode), portNode["Label"].as<std::string>());
		}
		// West ports.
		YAML::Node northPortList = portList["North Ports"];
		for (YAML::iterator portIterator = northPortList.begin(); portIterator != northPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			component->addPort(2, getPortType(portNode), portNode["Label"].as<std::string>());
		}
		// South ports.
		YAML::Node southPortList = portList["South Ports"];
		for (YAML::iterator portIterator = southPortList.begin(); portIterator != southPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			component->addPort(3, getPortType(portNode), portNode["Label"].as<std::string>());
		}

		// Add component to Scene.
		component->unhighlight();
		engine.m_circuit->m_components.push_back(component);
	}

	// ------------- //
	//  C A B L E S  //
	// ------------- //
}

//=============================================================================================================================================//
//  Utilities.				     																											   //
//=============================================================================================================================================//

PortType getPortType(YAML::Node node) 
{
	if		(node["Type"].as<std::string>() == "PORT_IN")	 { return PORT_IN; }
	else if (node["Type"].as<std::string>() == "PORT_OUT")	 { return PORT_OUT; }
	else if (node["Type"].as<std::string>() == "PORT_INOUT") { return PORT_INOUT; }
}

//=============================================================================================================================================//
//  EOF.				     																												   //
//=============================================================================================================================================//
