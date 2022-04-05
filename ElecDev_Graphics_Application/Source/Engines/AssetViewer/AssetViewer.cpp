//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/AssetViewer/AssetViewer.h"
#include "OpenGL/Primitives/Grid.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Graphics/Entities/EntityManager.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Utilities/Logger/Logger.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "imgui/imgui.h"

//=============================================================================================================================================//
//  Methods.																														           //
//=============================================================================================================================================//

AssetViewer::AssetViewer() 
{
	enableDesignPalette();
	m_scene->m_grid->disableHelperCircle();
}

void AssetViewer::clearAssets()
{
	bool bindScene = false;
	if (Renderer::getScene() != m_scene.get())
			bindScene = true;

	if(bindScene)
		Renderer::storeAndBindScene(m_scene.get());

	m_currentAsset = "No Asset.";
	m_circuit.reset();
	m_component.reset();

	if(bindScene)
		Renderer::restoreAndUnbindScene();
}

void AssetViewer::renderDesignPalette() 
{
	ImGui::SetCursorPosX(m_contentRegionSize.x / 2 - ImGui::CalcTextSize(m_currentAsset.c_str()).x / 2);
	ImGui::Text(m_currentAsset.c_str());
}

void AssetViewer::viewAsset(const std::filesystem::path& path)
{
	Renderer::storeAndBindScene(m_scene.get());

	try
	{
		// Create yaml node from file.
		YAML::Node yamlFile = YAML::LoadFile(path.string());

		// Circuits.
		if (yamlFile["Lumen File Info"]["Type"].as<std::string>() == "Circuit")
		{
			viewCircuit(yamlFile);
		}

		// Components.
		else if (yamlFile["Lumen File Info"]["Type"].as<std::string>() == "Component")
		{

		}

		m_currentAsset = path.filename().string();
	}
	catch (...)
	{
		LUMEN_LOG_ERROR("Could not load file.  It may contain invalid content or be an unsupported version.", "YAML Serialiser");
		return;
	}

	Renderer::restoreAndUnbindScene();
}

void AssetViewer::viewCircuit(const YAML::Node& yamlNode)
{
	clearAssets();

	// An ID table that holds the reference between new and old ID's.
	std::map<unsigned, unsigned> idTable;	// Old -> New

	// --------------- //
	//  C I R C U I T  //
	// --------------- //

	// Load the circuit info.
	YAML::Node circuitInfo = yamlNode["Circuit Info"];

	// Create an engine with the circuit.
	AssetViewer* engine = this;
	engine->m_circuit = std::make_unique<Circuit>(circuitInfo["Label"].as<std::string>(), circuitInfo["Type"].as<std::string>());

	// -------------------- //
	// C O M P O N E N T S  //
	// -------------------- //

	// Iterate through all of the components.
	YAML::Node componentList = yamlNode["Components"];
	for (YAML::iterator compIterator = componentList.begin(); compIterator != componentList.end(); ++compIterator)
	{
		YAML::Node componentNode = compIterator->second;
		// Create component.
		std::shared_ptr<Component2D> component = std::make_shared<Component2D>(engine->m_circuit.get());
		// Add component to circuit.
		engine->m_circuit->m_components.push_back(component);

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
			component->dataDict.insert({ it->first.as<std::string>(), it->second.as<std::string>() });
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
			unsigned entityID = component->addPort(1, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
		}
		// West ports.
		YAML::Node westPortList = portList["West Ports"];
		for (YAML::iterator portIterator = westPortList.begin(); portIterator != westPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			unsigned entityID = component->addPort(0, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
		}
		// West ports.
		YAML::Node northPortList = portList["North Ports"];
		for (YAML::iterator portIterator = northPortList.begin(); portIterator != northPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			unsigned entityID = component->addPort(2, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });
		}
		// South ports.
		YAML::Node southPortList = portList["South Ports"];
		for (YAML::iterator portIterator = southPortList.begin(); portIterator != southPortList.end(); ++portIterator)
		{
			YAML::Node portNode = portIterator->second;
			unsigned entityID = component->addPort(3, getPortType(portNode), portNode["Label"].as<std::string>());
			// Add entity ID to table.
			idTable.insert({ portNode["Entity ID"].as<unsigned>(), entityID });

		}

		// Remove compoennt highlightes.
		component->unhighlight();
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
			glm::vec2 node = { nodeList[nodeName][0].as<float>(), nodeList[nodeName][1].as<float>(), };
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
			engine->m_circuit.get(), titleString);
		cable->unhighlight();

		// Dictionary.
		cable->cableDict.clear();
		YAML::Node componentDict = cableNode["Dictionary"];
		for (YAML::iterator it = componentDict.begin(); it != componentDict.end(); ++it)
		{
			cable->cableDict.insert({ it->first.as<std::string>(), it->second.as<std::string>() });
		}

		// Add cable to circuit.
		engine->m_circuit->m_cables.push_back(cable);
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//