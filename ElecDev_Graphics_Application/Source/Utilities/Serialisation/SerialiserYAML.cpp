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

void saveToYAML(std::shared_ptr<Circuit> circuit, std::string directory)
{
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
	
	// --------- //
	//  S A V E  //
	// --------- //
	
	// End file.
	yamlEmitter << YAML::EndMap;
	// Store file.
	std::ofstream yamlStream;
	std::string file = directory + "\\" + circuit->m_label + ".lmn";
	yamlStream.open(file);
	yamlStream << yamlEmitter.c_str();
	yamlStream.close();
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