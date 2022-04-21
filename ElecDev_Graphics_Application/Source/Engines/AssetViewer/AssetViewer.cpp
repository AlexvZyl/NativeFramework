//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/AssetViewer/AssetViewer.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Graphics/Entities/EntityManager.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Utilities/Logger/Logger.h"
#include "OpenGL/Primitives/Grid.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "imgui/imgui.h"
#include "Application/LumenWindow/LumenWindow.h"

//=============================================================================================================================================//
//  Methods.																														           //
//=============================================================================================================================================//

AssetViewer::AssetViewer() 
{
	enableDesignPalette();
	getScene().getGrid().disableHelperCircle();
	getScene().getCamera().scale2D(0.5f);
}

void AssetViewer::clearAssets()
{
	Renderer::storeAndBindScene(&getScene());

	m_currentAsset = "No Asset.";
	m_circuit.reset();
	m_component.reset();

	Renderer::restoreAndUnbindScene();
}

void AssetViewer::renderDesignPalette() 
{
	ImGui::SetCursorPosX(getWindowContentRegionSize().x / 2 - ImGui::CalcTextSize(m_currentAsset.c_str()).x / 2);
	ImGui::Text(m_currentAsset.c_str());
}

void AssetViewer::viewAsset(const std::filesystem::path& path)
{
	// Check if asset is already being viewed.
	if (path.filename().string() == m_currentAsset)
		return;
	
	Renderer::storeAndBindScene(&getScene());

	try
	{
		// Circuits.
		if (path.extension() == ".lmct")
		{
			viewCircuit(path);
		}

		// Components.
		else if (path.extension() == ".lmcp")
		{
			viewComponent(path);
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

void AssetViewer::viewCircuit(const std::filesystem::path& path)
{
	clearAssets();
	m_circuit = std::make_unique<Circuit>(path);
}

void AssetViewer::viewComponent(const std::filesystem::path& path)
{
	clearAssets();
	m_component = std::make_unique<Component2D>(path);
	m_component->disableOutline();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//