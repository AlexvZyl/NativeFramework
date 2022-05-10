#pragma once

//=============================================================================================================================================//
//  Includes. 																																   //
//=============================================================================================================================================//

#include "Graphics/Entities/Entity.h"
#include "Cable.h"
#include "Component2D.h"
#include "yaml-cpp/yaml.h"
#include <filesystem>

//=============================================================================================================================================//
//  Circuit Class.    																														   //
//=============================================================================================================================================//

class Circuit : public Entity
{    
public:

    // Constructors.
    Circuit(std::string label, std::string type);
    Circuit(const std::filesystem::path& path);
    Circuit(const YAML::Node& node);
    // Destructor.
    ~Circuit();

    // Entities.
    std::vector<std::shared_ptr<Component2D>> m_components;
    std::vector<std::shared_ptr<Cable>> m_cables;

    // Metadata.
    std::string m_label;
    std::string m_type;

    // Keep track of imported entities to be saved with the circuit.
    std::unordered_map<std::string, YAML::Node> m_uniqueComponents;
    std::unordered_map<std::string, YAML::Node> m_uniqueCables;

};

//=============================================================================================================================================//
//  EOF.    																																   //
//=============================================================================================================================================//