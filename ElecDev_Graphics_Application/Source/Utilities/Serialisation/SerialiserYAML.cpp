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

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

void saveToYAML(std::shared_ptr<Circuit>& circuit, std::string& directory, std::string& filename)
{
	std::string labelTemp;
	// Check if file name supplied.
	if (filename.length()) 
	{
		// Remove extensions from the file name.
		if		(filename.find(".yml") != std::string::npos)	{ filename.erase(filename.length() - 4, 4); }
		else if (filename.find(".yaml") != std::string::npos)	{ filename.erase(filename.length() - 5, 5); }
		else if (filename.find(".lmct") != std::string::npos)	{ filename.erase(filename.length() - 5, 5); }
		// Change circuit name to the file name.
		labelTemp = circuit->m_label;	
		circuit->m_label = filename;
	}

	// --------- //
	//  F I L E  //
	// --------- //

	// Begin File.
	YAML::Emitter yamlEmitter;
	yamlEmitter << YAML::BeginMap;

	yamlEmitter << YAML::Key << "Lumen File Info" << YAML::Value;
	yamlEmitter << YAML::BeginMap;
		yamlEmitter << YAML::Key << "Version" << YAML::Value << "0.0.1";
		yamlEmitter << YAML::Key << "Type" << YAML::Value << "Circuit";
	yamlEmitter << YAML::EndMap;

	// Serialise circuit.
	yamlEmitter << circuit;

	// Restore current (in Lumen) circuit name.
	if (filename.length()) 
		circuit->m_label = labelTemp;
	
	// --------- //
	//  S A V E  //
	// --------- //

	// Make sure directory has backslash.
	if (directory.back() != '\\') { directory.push_back('\\'); }
	
	std::string file;
	// If filename was not supplied, create from circuit.
	if (!filename.length()) 
	{ 
		file = directory + circuit->m_label + ".lmct"; 
	}
	// Use supplied filename.
	else 
	{
		std::string ext = ".lmct";
		filename.insert(filename.end(), ext.begin(), ext.end());
		// Set file name;
		file = directory + filename;
	}

	// End YAML.
	yamlEmitter << YAML::EndMap;
	// Save file.
	std::ofstream yamlStream;
	yamlStream.open(file);
	yamlStream << yamlEmitter.c_str();
	yamlStream.close();
}

//=============================================================================================================================================//
//  Deserialisation.		  																												   //
//=============================================================================================================================================//

void loadFromYAML(std::string& path)
{
	// Create yaml node from file.
	YAML::Node yamlFile = YAML::LoadFile(path);

	// Deserialise the circuit into the engine.
	if (yamlFile["Lumen File Info"]["Type"].as<std::string>() == "Circuit")
	{
		deserialiseCircuit(yamlFile);
	}
}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//