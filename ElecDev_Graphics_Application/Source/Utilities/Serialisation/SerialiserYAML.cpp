//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <sys/stat.h>
#include <iostream>
#include <direct.h>
#include <fstream>
#include "Serialiser.h"
#include <Windows.h>
#include "Utilities/Windows/WindowsUtilities.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Graphics/Fonts/FontLoader.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"
#include "Engines/Design2DEngine/ComponentDesigner.h"

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

void saveToYAML(std::shared_ptr<Circuit>& circuit, const std::filesystem::path& path)
{
	// Change the circuit name if a file name is supplied.
	if (path.filename().string().size())
	{
		std::string newName = path.filename().stem().string();
		circuit->m_label = newName;
	}

	// Create yaml file.
	YAML::Emitter yamlEmitter;
	yamlEmitter << YAML::BeginMap;
		yamlEmitter << YAML::Key << "Lumen File Info" << YAML::Value;
		yamlEmitter << YAML::BeginMap;
			yamlEmitter << YAML::Key << "Version" << YAML::Value << "0.0.1";
			yamlEmitter << YAML::Key << "Type" << YAML::Value << "Circuit";
		yamlEmitter << YAML::EndMap;
		yamlEmitter << circuit;
	yamlEmitter << YAML::EndMap;

	// If a directory was received and not a file, create a file from the component label.
	std::string saveLocation = path.string();
	if (!path.filename().string().size())
	{
		saveLocation += circuit->m_label + ".lmct";
	}
	// Check if a file extension was supplied.
	else if (path.extension().string() != ".lmct")
	{
		saveLocation += ".lmct";
	}

	// Save file.
	std::ofstream yamlStream;
	yamlStream.open(saveLocation);
	yamlStream << yamlEmitter.c_str();
	yamlStream.close();
}

void saveToYAML(std::shared_ptr<Component2D>& component, const std::filesystem::path& path)
{
	// Change the component name if a file name is supplied.
	if (path.filename().string().size())
	{
		std::string newName = path.filename().stem().string();
		component->titleString = newName;
		component->title->updateText(newName);
	}

	// Create yaml file.
	YAML::Emitter yamlEmitter;
	yamlEmitter << YAML::BeginMap;
		yamlEmitter << YAML::Key << "Lumen File Info" << YAML::Value;
		yamlEmitter << YAML::BeginMap;
			yamlEmitter << YAML::Key << "Version" << YAML::Value << "0.0.1";
			yamlEmitter << YAML::Key << "Type" << YAML::Value << "Component";
		yamlEmitter << YAML::EndMap;
		yamlEmitter << YAML::Key << "Component" << YAML::Value << component;
	yamlEmitter << YAML::EndMap;

	// If a directory was received and not a file, create a file from the component label.
	std::string saveLocation = path.string();
	if (!path.filename().string().size())
	{
		saveLocation += component->titleString + ".lmcp";
	}
	// Check if a file extension was supplied.
	else if (path.extension().string() != ".lmcp")
	{
		saveLocation += ".lmcp";
	}

	// Save the yaml file.
	std::ofstream yamlStream;
	yamlStream.open(saveLocation);
	yamlStream << yamlEmitter.c_str();
	yamlStream.close();
}

//=============================================================================================================================================//
//  Deserialisation.		  																												   //
//=============================================================================================================================================//

void loadFromYAML(const std::filesystem::path& path)
{
	try
	{
		// Circuits.
		if (path.extension() == ".lmct")
		{
			//deserialiseCircuit(yamlFile, true);
			Design2DEngine* engine = Lumen::getApp().pushEngineLayer<Design2DEngine>(path.stem().string())->getEngine();
			engine->createCircuit(path);
		}

		// Components.
		else if (path.extension() == ".lmcp")
		{
			ComponentDesigner* engine = Lumen::getApp().pushEngineLayer<ComponentDesigner>(path.stem().string())->getEngine();
			engine->setComponent(path);
		}
	}
	catch (...)
	{
		LUMEN_LOG_ERROR("Could not load file.  It may contain invalid content or be an unsupported version.", "YAML Serialiser");
		return;
	}
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//