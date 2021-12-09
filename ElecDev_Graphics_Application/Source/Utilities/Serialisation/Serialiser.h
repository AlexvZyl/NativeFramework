#pragma once

/*
Handles the serialising and deserialising of data.  First use case will be for
storing circuit templates/diagrams in a YAML format. 
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <string>
#include "External/GLM/glm.hpp"
#include "External/YAML-CPP/Includes/yaml-cpp/yaml.h"
#include "Graphics/OpenGL/Design2DEngineGL/Peripherals/Port.h"

//=============================================================================================================================================//
//  Forward decelrations.																													   //
//=============================================================================================================================================//

class Circuit;
class Design2DEngineGL;

static std::string assetsFolder = "Assets\\";

//=============================================================================================================================================//
//  Serialisation.																															   //
//=============================================================================================================================================//

// Store a circuit in a YAML file.
void saveToYAML(Circuit& circuit, std::string folder = "");
// Store a circuit in a BIN file.
void saveToBIN(Circuit& circuit, std::string folder = "");

//=============================================================================================================================================//
//  Deserialisation.																														   //
//=============================================================================================================================================//

// Load a circuit from YAML file.
void loadFromYAML(Design2DEngineGL& engine, std::string file);
// Load a circuit from BIN file.
void loadFromBIN(Design2DEngineGL& engine, std::string file);

//=============================================================================================================================================//
//  Utilities.																																   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can work with port types.
inline YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type);
// Overloads the << operator so that YAML-cpp can accept glm::vec2.
inline YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2 vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec3.
inline YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3 vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec4.
inline YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4 vec);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//