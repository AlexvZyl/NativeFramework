//=============================================================================================================================================//
//  Includes.			     																												   //
//=============================================================================================================================================//

#include "../Serialiser.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Engines/Design2DEngine/Design2Dengine.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"
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

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Circuit>& circuit) 
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

YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Circuit>>& circuitVector) 
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

std::shared_ptr<Circuit> deserialiseCircuit(YAML::Node& yamlNode, bool newEngine)
{
	// An ID table that holds the reference between new and old ID's.
	std::map<unsigned, unsigned> idTable;	// Old -> New

	// --------------- //
	//  C I R C U I T  //
	// --------------- //

	// Load the circuit info.
	YAML::Node circuitInfo = yamlNode["Circuit Info"];
	std::shared_ptr<Circuit> circuit = std::make_shared<Circuit>(circuitInfo["Label"].as<std::string>(),
																	 circuitInfo["Type"].as<std::string>());
	// Create an engine with the circuit.
	if (newEngine)
	{
		Design2DEngine* engine = Lumen::getApp().pushEngineLayer<Design2DEngine>(circuit->m_label)->getEngine();
		engine->m_circuit = circuit;
	}
	
	// -------------------- //
	// C O M P O N E N T S  //
	// -------------------- //

	// Iterate through all of the components.
	YAML::Node componentList = yamlNode["Components"];
	for (YAML::iterator compIterator = componentList.begin(); compIterator != componentList.end(); ++compIterator)
	{
		YAML::Node componentNode = compIterator->second;
		// Create component.
		std::shared_ptr<Component2D> component = std::make_shared<Component2D>(circuit.get());
		// Add component to circuit.
		circuit->m_components.push_back(component);

		// Add entity ID to table.
		idTable.insert({ componentNode["Entity ID"].as<unsigned>(), component->m_entityID });

		// Load data into component.
		glm::vec2 position(
			componentNode["Centre"][0].as<float>(),
			componentNode["Centre"][1].as<float>()
		);
		component->place(position);
		// Update title.
		std::string compTitle = componentNode["Title"]["String"].as<std::string>();
		component->title->updateText(compTitle);
		component->titleString = compTitle;

		// Dictionary.
		component->dataDict.clear();
		YAML::Node componentDict = componentNode["Dictionary"];
		for (YAML::iterator it = componentDict.begin(); it != componentDict.end(); ++it)
		{
			component->dataDict.insert({it->first.as<std::string>(), it->second.as<std::string>()});
		}

		// ----------- //
		//  P O R T S  //
		// ----------- //
		
		// Load the ports.
		YAML::Node portList = componentNode["Ports"];

		// East ports.
		YAML::Node eastPortList = portList["East Ports"];
		for (YAML::iterator portIterator = eastPortList.begin(); portIterator != eastPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			//unsigned entityID = component->addPort(1, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
		}
		// West ports.
		YAML::Node westPortList = portList["West Ports"];
		for (YAML::iterator portIterator = westPortList.begin(); portIterator != westPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			//unsigned entityID = component->addPort(0, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
		}
		// West ports.
		YAML::Node northPortList = portList["North Ports"];
		for (YAML::iterator portIterator = northPortList.begin(); portIterator != northPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			//unsigned entityID = component->addPort(2, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
		}
		// South ports.
		YAML::Node southPortList = portList["South Ports"];
		for (YAML::iterator portIterator = southPortList.begin(); portIterator != southPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			//unsigned entityID = component->addPort(3, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			//idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });

		}

		// Remove compoennt highlightes.
		component->disableOutline();
	}

	// ------------- //
	//  C A B L E S  //
	// ------------- //

	// Iterate through all of the cables.
	YAML::Node cableList = yamlNode["Cables"];
	for (YAML::iterator cableIterator = cableList.begin(); cableIterator != cableList.end(); ++cableIterator)
	{
		// load the YAML node.
		YAML::Node cableNode = cableIterator->second;

		// Load the node vector.
		std::vector<glm::vec2> nodeVector;
		YAML::Node nodeList = cableNode["Nodes"];
		for (int i = 0; i < nodeList.size(); i++) 
		{
			// Add to node vector.
			std::string nodeName = "Node " + std::to_string(i);
			glm::vec2 node = { nodeList[nodeName][0].as<float>(), nodeList[nodeName][1].as<float>(),};
			nodeVector.push_back(node);
		}

		// Find connected prorts.
		Port* startPort = dynamic_cast<Port*>(EntityManager::getEntity(idTable[cableNode["Start port"].as<unsigned>()]));
		Port* endPort = dynamic_cast<Port*>(EntityManager::getEntity(idTable[cableNode["End port"].as<unsigned>()]));

		// Get the title details.
		std::string titleString = cableNode["Title"].as<std::string>();
		//glm::vec3 title1pos = { cableNode["Title1 pos"][0].as<float>(), cableNode["Title1 pos"][1].as<float>(), cableNode["Title1 pos"][2].as<float>() };
		//glm::vec3 title2pos = { cableNode["Title2 pos"][0].as<float>(), cableNode["Title2 pos"][1].as<float>(), cableNode["Title2 pos"][2].as<float>() };
		// Create cable.
		std::shared_ptr<Cable> cable = std::make_shared<Cable>(startPort,
															   nodeVector,
															   endPort,
															   circuit.get(), titleString);
		cable->disableOutline();

		// Dictionary.
		cable->cableDict.clear();
		YAML::Node componentDict = cableNode["Dictionary"];
		for (YAML::iterator it = componentDict.begin(); it != componentDict.end(); ++it)
		{
			cable->cableDict.insert({ it->first.as<std::string>(), it->second.as<std::string>() });
		}

		// Add cable to circuit.
		circuit->m_cables.push_back(cable);
	}

	return circuit;
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
