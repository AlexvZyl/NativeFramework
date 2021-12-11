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

// Classes.
class Circuit;
class Design2DEngineGL;
class Port;
class Component2D;
class Cable;

// Enums.
enum PortType;

//=============================================================================================================================================//
//  Static variables.																														   //
//=============================================================================================================================================//

static std::string assetsFolder = "Assets\\";

//=============================================================================================================================================//
//  GLM.																																	   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can accept glm::vec2.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec2 vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec3.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3 vec);
// Overloads the << operator so that YAML-cpp can accept glm::vec4.
YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec4 vec);

//=============================================================================================================================================//
//  Circuits.																																   //
//=============================================================================================================================================//

// Serialise a circuit to a YAML file.
void saveToYAML(std::shared_ptr<Circuit> circuit, std::string path);
// Serialise a circuit to a BIN file.
void saveToBIN(std::shared_ptr<Circuit> circuit, std::string folder = "");
// Deserialise a circuit from a YAML file.
void loadFromYAML(Design2DEngineGL& engine, std::string file, std::string folder = "");
// Deserialise a circuit from a BIN file.
void loadFromBIN(Design2DEngineGL& engine, std::string file, std::string folder = "");
// Load a circuit from a YAML file.
void loadCircuitFromYAML(std::string path, Design2DEngineGL* engine);
// Overloads the << operator so that YAML-cpp can work with circuits.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Circuit> circuit);
// Overloads the << operator so that YAML-cpp can work with circuits.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Circuit>> circuitVector);

//=============================================================================================================================================//
//  Ports.																																	   //
//=============================================================================================================================================//

// Overloads the << operator so that YAML-cpp can work with port types.
YAML::Emitter& operator<<(YAML::Emitter& emitter, PortType type);
// Serialise a single port.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Port> port);
// Serialise a port vector.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Port>> portVector);

//=============================================================================================================================================//
//  Componenet 2D.																															   //
//=============================================================================================================================================//

// Serialise a single Component2D.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Component2D> comp);
// Serialise a Component2D vecrtor.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Component2D>> compVector);
// Deserialise a single Componenet2D.
void deserialise(YAML::Node yamlComp, std::shared_ptr<Component2D> component);

//=============================================================================================================================================//
//  Cables.																																	   //
//=============================================================================================================================================//

// Serialise a single cable.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::shared_ptr<Cable> cable);
// Serialise a Cable vector.
YAML::Emitter& operator<<(YAML::Emitter& emitter, std::vector<std::shared_ptr<Cable>> cableVector);

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//