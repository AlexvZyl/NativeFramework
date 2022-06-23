//==============================================================================================================================================//
//  Includes.  																																    //
//==============================================================================================================================================//

#include "Circuit.h"
#include "Component2D.h"
#include "Cable.h"

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
		YAML::Node componentNode = component.second;

		// Create the component from the node.
		std::string componentFile = componentNode["Filename"].as<std::string>();
		m_components.emplace_back(std::make_shared<Component2D>(m_referenceComponents[componentFile], this));

		// Update component.
		auto& currentComponent = m_components.back();
		currentComponent->disableOutline();
		currentComponent->translateTo({ componentNode["Position"][0].as<float>(), componentNode["Position"][1].as<float>() });
		currentComponent->dataDict.clear();
		YAML::Node dictNode = component.second["Dictionary"];
		for (const auto& node : dictNode)
		{
			currentComponent->dataDict.insert({ node.first.as<std::string>(), node.second.as<std::string>() });
		}

		if (componentNode["Designator Index"].IsDefined())
		{
			currentComponent->designatorIdx = componentNode["Designator Index"].as<int>();
			currentComponent->updateText();
		}
		else 
		{
			currentComponent->designatorIdx = -1;
			currentComponent->updateText();
		}
		
		// Rotate the component.
		if (componentNode["Rotation"].IsDefined())
			currentComponent->rotate(componentNode["Rotation"].as<float>());

		// Add tag.
		if (componentNode["Tag"].IsDefined()) {
			currentComponent->setTag(componentNode["Tag"].as<std::string>());
		}
	}

	// Load cables.
	for (const auto& cable : node["Cables"])
	{
		m_cables.push_back(std::make_shared<Cable>(cable.second, this));
		std::string filename = cable.second["Filename"].as<std::string>();
		YAML::Node cableNode = node["Reference Cables"][filename];
	}
}

//==============================================================================================================================================//
//  EOF.    																																    //
//==============================================================================================================================================//