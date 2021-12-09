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

//=============================================================================================================================================//
//  Forward decelrations.																													   //
//=============================================================================================================================================//

class Circuit;
class Design2DEngineGL;
enum PortType;
class Port;
class Component2D;
class Cable;

//=============================================================================================================================================//
//  Static variables.																														   //
//=============================================================================================================================================//

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
void loadFromYAML(Design2DEngineGL& engine, std::string file, std::string folder = "");
// Load a circuit from BIN file.
void loadFromBIN(Design2DEngineGL& engine, std::string file, std::string folder = "");

//=============================================================================================================================================//
//  Utilities.																																   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can work with port types.
YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type);
// Overloads the << operator so that YAML-cpp can accept glm::vec2.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2 vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec3.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3 vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec4.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4 vec);

//=============================================================================================================================================//
//  Components.																																   //
//=============================================================================================================================================//

// Serialise a single port.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Port> port);
// Serialise a port vector.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Port>> portVector);
// Serialise a single Component2D.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Component2D> comp);
// Serialise a Component2D vecrtor.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>> compVector);
// Serialise a single cable.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Cable> cable);
// Serialise a Cable vector.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Cable>> cableVector);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//