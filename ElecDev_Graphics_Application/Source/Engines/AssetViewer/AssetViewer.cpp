//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Engines/AssetViewer/AssetViewer.h"
#include "Engines/CircuitDesigner/Peripherals/Component2D.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
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
	enableMenuBar();
	getScene().getGrid()
		.disableHelperCircle()
		.disable();
	getScene().getCamera().scale2D(100.f);
}

void AssetViewer::clearAssets()
{
	Renderer::storeAndBindScene(&getScene());
	m_currentAsset = "No Asset.";
	m_circuit = nullptr;
	m_component = nullptr;
	Renderer::restoreAndUnbindScene();
}

void AssetViewer::renderMenuBar() 
{
	ImGui::SetCursorPosX(getWindowContentRegionSize().x / 2 - ImGui::CalcTextSize(m_currentAsset.c_str()).x / 2);
	ImGui::Text(m_currentAsset.c_str());
}

void AssetViewer::viewAsset(const std::filesystem::path& path)
{
	// Check if asset is already being viewed.
	if (path.filename().string() == m_currentAsset) return;

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
		// Error.
		else
		{
			LUMEN_LOG_ERROR("Unsupported file extension.", "YAML Serialiser");
		}
	}
	catch (...)
	{
		LUMEN_LOG_ERROR("Could not load file.  It may contain invalid content or be an unsupported version.", "YAML Serialiser");
		return;
	}
}

void AssetViewer::viewCircuit(const std::filesystem::path& path)
{
	Renderer::storeAndBindScene(&getScene());
	clearAssets();
	m_circuit = std::make_unique<Circuit>(path);
	m_currentAsset = path.filename().string();
	Renderer::restoreAndUnbindScene();
}

void AssetViewer::viewCircuit(const YAML::Node& node)
{
	Renderer::storeAndBindScene(&getScene());
	clearAssets();
	m_circuit = std::make_unique<Circuit>(node);
	m_currentAsset = m_circuit->m_label + ".lmct";
	Renderer::restoreAndUnbindScene();
}

void AssetViewer::viewComponent(const std::filesystem::path& path)
{
	Renderer::storeAndBindScene(&getScene());
	clearAssets();
	m_component = std::make_unique<Component2D>(path);
	m_component->disableOutline();
	m_currentAsset = path.filename().string();
	Renderer::restoreAndUnbindScene();
}

void AssetViewer::viewComponent(const YAML::Node& node)
{
	Renderer::storeAndBindScene(&getScene());
	clearAssets();
	m_component = std::make_unique<Component2D>(node);
	m_component->disableOutline();
	m_currentAsset = m_component->equipType + ".lmcp";
	Renderer::restoreAndUnbindScene();
}

void AssetViewer::onFocusEventForce(const NotifyEvent& event) 
{
	// Do not bind the scene on a focus.
	m_isFocused = true;
	onFocusEvent(event);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//