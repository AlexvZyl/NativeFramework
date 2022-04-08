//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/AssetViewer/AssetViewer.h"
#include "OpenGL/Primitives/Grid.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Graphics/Entities/EntityManager.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Utilities/Logger/Logger.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "imgui/imgui.h"

//=============================================================================================================================================//
//  Methods.																														           //
//=============================================================================================================================================//

AssetViewer::AssetViewer() 
{
	enableDesignPalette();
	m_scene->m_grid->disableHelperCircle();
}

void AssetViewer::clearAssets()
{
	Renderer::storeAndBindScene(m_scene.get());

	m_currentAsset = "No Asset.";
	m_circuit.reset();
	m_component.reset();

	Renderer::restoreAndUnbindScene();
}

void AssetViewer::renderDesignPalette() 
{
	ImGui::SetCursorPosX(m_contentRegionSize.x / 2 - ImGui::CalcTextSize(m_currentAsset.c_str()).x / 2);
	ImGui::Text(m_currentAsset.c_str());
}

void AssetViewer::viewAsset(const std::filesystem::path& path)
{
	Renderer::storeAndBindScene(m_scene.get());

	try
	{
		// Create yaml node from file.
		YAML::Node yamlFile = YAML::LoadFile(path.string());

		// Circuits.
		if (path.extension() == ".lmct")
		{
			viewCircuit(yamlFile);
		}

		// Components.
		else if (path.extension() == ".lmcp")
		{
			viewComponent(yamlFile);
		}

		m_currentAsset = path.filename().string();
	}
	catch (...)
	{
		LUMEN_LOG_ERROR("Could not load file.  It may contain invalid content or be an unsupported version.", "YAML Serialiser");
		return;
	}

	Renderer::restoreAndUnbindScene();
}

void AssetViewer::viewCircuit(YAML::Node& yamlNode)
{
	clearAssets();
	m_circuit = deserialiseCircuit(yamlNode);
}

void AssetViewer::viewComponent(YAML::Node& yamlNode)
{
	clearAssets();
	m_component = std::make_unique<Component2D>(yamlNode);
	m_component->disableOutline();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//