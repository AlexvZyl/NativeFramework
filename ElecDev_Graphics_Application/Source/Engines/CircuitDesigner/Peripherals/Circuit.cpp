//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Circuit.h"
#include "Component2D.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																												    //
//==============================================================================================================================================//

Circuit::Circuit(std::string label, std::string type)
	: Entity(EntityType::CIRCUIT), m_label(label), m_type(type)
{}

Circuit::~Circuit()
{
	m_components.clear();
	m_cables.clear();
}

Circuit::Circuit(const std::filesystem::path& path)
	: Circuit(YAML::LoadFile(path.string()))
{}

Circuit::Circuit(const YAML::Node& node) 
	: Entity(EntityType::CIRCUIT)
{
	m_label = node["Circuit Info"]["Label"].as<std::string>();

	// Load reference components.
	for (const auto& component : node["Reference Components"])
	{
		m_referenceComponents.insert({ component.first.as<std::string>(), component.second });;
	}

	// Load reference cables.
	for (const auto& cable : node["Reference Cables"])
	{
		m_referenceCables.insert({ cable.first.as<std::string>(), cable.second });
	}

	// Load components.
	for (const auto& component : node["Components"])
	{
		// Create the component from the node.
		std::string componentFile = component.second["Filename"].as<std::string>();
		m_components.emplace_back(std::make_shared<Component2D>(m_referenceComponents[componentFile], this));

		// Update component.
		auto& currentComponent = m_components.back();
		currentComponent->disableOutline();
		currentComponent->moveTo({ component.second["Position"][0].as<float>(), component.second["Position"][1].as<float>() });
		currentComponent->dataDict.clear();
		YAML::Node dictNode = component.second["Dictionary"];
		for (const auto& node : dictNode)
		{
			currentComponent->dataDict.insert({ node.first.as<std::string>(), node.second.as<std::string>() });
		}
		std::string label = component.second["Label"].as<std::string>();
		currentComponent->titleString = label;
		currentComponent->title->updateText(label);
	}

	// Load cables.
	for (const auto& cable : node["Cables"])
	{
		m_cables.push_back(std::make_shared<Cable>(cable.second, this));
		std::string filename = cable.second["Filename"].as<std::string>();
		YAML::Node cableNode = node["Reference Cables"][filename];
		glm::vec4 cableColor = { cableNode["Color"][0].as<float>(), cableNode["Color"][1].as<float>(), cableNode["Color"][2].as<float>(), cableNode["Color"][3].as<float>() };
		m_cables.back()->setColour(cableColor);
	}
}

//==============================================================================================================================================//
//  EOF.    																																    //
//==============================================================================================================================================//