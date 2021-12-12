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

void saveToYAML(std::shared_ptr<Circuit> circuit, std::string directory, std::string filename)
{
	std::string labelTemp;
	// Check if file name supplied.
	if (filename.length()) 
	{
		// Remove extensions from the file name.
		if		(filename.find(".yml") != std::string::npos)	{ filename.erase(filename.length() - 4, 4); }
		else if (filename.find(".yaml") != std::string::npos)	{ filename.erase(filename.length() - 5, 5); }
		else if (filename.find(".lmn") != std::string::npos)	{ filename.erase(filename.length() - 4, 4); }
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

	// Restore circuit label name.
	if (filename.length()) { circuit->m_label = labelTemp; }
	
	// --------- //
	//  S A V E  //
	// --------- //

	// Make sure directory has backslach.
	if (directory.back() != '\\') { directory.push_back('\\'); }
	
	std::string file;
	// Check if filename was supplied.
	if (!filename.length()) 
	{ 
		file = directory + circuit->m_label + ".lmn"; 
	}
	else 
	{
		std::string ext = ".lmn";
		filename.insert(filename.end(), ext.begin(), ext.end());
		// Set file name;
		file = directory + filename;
	}

	// End file.
	yamlEmitter << YAML::EndMap;
	// Store file.
	std::ofstream yamlStream;
	yamlStream.open(file);
	yamlStream << yamlEmitter.c_str();
	yamlStream.close();
}

//=============================================================================================================================================//
//  Deserialisation.																														   //
//=============================================================================================================================================//

void loadFromYAML(Design2DEngineGL& engine, std::string directory, std::string file)
{
	// Make sure directory has backslach.
	if (directory.back() != '\\') { directory.push_back('\\'); }

	// Create yaml node from file.
	YAML::Node yamlFile = YAML::LoadFile(directory + file);

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
	for (YAML::iterator comp = componentList.begin(); comp != componentList.end(); ++comp)
	{
		// Create component.
		std::shared_ptr<Component2D> component = std::make_shared<Component2D>(engine.m_trianglesVAO.get(),
																			   engine.m_linesVAO.get(),
																			   engine.m_texturedTrianglesVAO.get(),
																			   engine.m_circlesVAO.get(),
																			   engine.m_circuit.get());
		// Load data into component.
		deserialise(comp->second, component);	// The second part of the iterator is the YAML Node.
		// Push onto componenets vector.
		engine.m_circuit->m_components.push_back(component);
	}

	// ------------- //
	//  C A B L E S  //
	// ------------- //


}


//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//