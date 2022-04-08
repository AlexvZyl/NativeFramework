#pragma once

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/Base2DEngine/Base2DEngine.h"
#include "External/YAML-CPP/Includes/yaml-cpp/yaml.h"
#include <filesystem>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Circuit;
class Component2D;

//=============================================================================================================================================//
//  EngineCore Class.																														   //
//=============================================================================================================================================//

class AssetViewer : public Base2DEngine
{
public:

    // Constructor.
    AssetViewer();
    // Destructor.
    ~AssetViewer() = default;

    // So that the file name can be displayed.
    virtual void renderDesignPalette() override;

    // Asset viewing functions.
    void clearAssets();
    void viewCircuit(YAML::Node& node);
    void viewComponent(YAML::Node& node);
    void viewAsset(const std::filesystem::path& path);

    // Assets that can be viewed.
    std::shared_ptr<Circuit> m_circuit;
    std::unique_ptr<Component2D> m_component;
    std::string m_currentAsset = "No Asset.";
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//