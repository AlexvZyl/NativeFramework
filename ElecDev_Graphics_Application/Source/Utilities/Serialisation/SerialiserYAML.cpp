//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Serialiser.h"
#include <Windows.h>
#include "Utilities/Windows/WindowsUtilities.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Circuit.h"
#include "Graphics/OpenGL/Design2DEngineGL/Design2D_Engine.h"
#include "CoreGL/FontLoader.h"
#include <sys/stat.h>
#include <iostream>
#include <direct.h>
#include <fstream>

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

void saveToYAML(Circuit& circuit, std::string folder)
{
	// ------------- //
	//  F O L D E R  //
	// ------------- //

	std::string exeLoc = getExecutableLocation();
	// Check if folder is provided.
	if (folder.length())	
	{
		struct stat info;
		std::string folderPath = exeLoc + assetsFolder + folder;
		// If folder does not exist, create one.
		if (stat(folderPath.c_str(), &info)) { _mkdir(folderPath.c_str()); }
	}

	// Begin File.
	YAML::Emitter yamlEmitter;
	yamlEmitter << YAML::BeginMap;

	// --------------- //
	//  C I R C U I T  //
	// --------------- //

	// Circuit data.
	yamlEmitter << YAML::Key << "Circuit Info" << YAML::Value;
	yamlEmitter << YAML::BeginMap;
		yamlEmitter << YAML::Key << "Label" << YAML::Value << circuit.m_label;
		yamlEmitter << YAML::Key << "Type" << YAML::Value << circuit.m_type;
	yamlEmitter << YAML::EndMap;
	// Components.
	yamlEmitter << YAML::Key << "Components" << YAML::Value << circuit.m_components;
	// Cables.
	yamlEmitter << YAML::Key << "Cables" << YAML::Value << circuit.m_cables;
	
	// --------- //
	//  S A V E  //
	// --------- //
	
	// End file.
	yamlEmitter << YAML::EndMap;
	// Store file.
	const std::string yamlLocation = exeLoc + assetsFolder + folder + "\\" + circuit.m_label + ".lmn";
	std::ofstream yamlStream;
	yamlStream.open(yamlLocation);
	yamlStream << yamlEmitter.c_str();
}

//=============================================================================================================================================//
//  Deserialisation.																														   //
//=============================================================================================================================================//

void loadFromYAML(Design2DEngineGL& engine, std::string file, std::string folder)
{
	// Create yaml node from file.
	YAML::Node yamlFile = YAML::LoadFile(getExecutableLocation() + assetsFolder + folder + "\\" + file);

	// --------------- //
	//  C I R C U I T  //
	// --------------- //

	YAML::Node circuitInfo = yamlFile["Circuit Info"];
	engine.m_circuit.reset();
	engine.m_circuit = std::make_unique<Circuit>(
			circuitInfo["Label"].as<std::string>(),
			circuitInfo["Type"].as<std::string>()
		);

	// -------------------- //
	// C O M P O N E N T S  //
	// -------------------- //

	// Load all of the components.
    YAML::Node componentList = yamlFile["Components"];
	// Iterate through all of the components.
	for (YAML::iterator it = componentList.begin(); it != componentList.end(); ++it)
	{
		// Get component from iterator.
		YAML::Node comp = it->second;
		// Create component.
		engine.m_circuit->m_components.push_back
		(
			std::make_shared<Component2D>
			(
				engine.m_trianglesVAO.get(),
				engine.m_linesVAO.get(),
				engine.m_texturedTrianglesVAO.get(),
				engine.m_circlesVAO.get(),
				engine.m_circuit.get()
			)
		);
		// Assign component data.


		
	}

	// ------------- //
	//  C A B L E S  //
	// ------------- //


}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//