//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Application/Layers/LayerStack.h"
#include "Application/Layers/Layer.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"

//==============================================================================================================================================//
//  Layer management.																															//
//==============================================================================================================================================//

std::unordered_map<std::string, std::unique_ptr<Layer>>& LayerStack::getLayers()
{
	return m_layers;
}

void LayerStack::queuePopLayer(Layer& layer)
{
	m_layerPopQueue.push_back(&layer);
}

void LayerStack::popLayers()
{
	// Check if there are layers to remove.
	if (!m_layerPopQueue.size()) return;

	Application& app = Lumen::getApp();

	// Get active layers.
	Layer* hoveredLayer = app.m_hoveredLayer;
	Layer* focusedLayer = app.m_focusedLayer;

	// Remove layers.
	for (Layer* layer : m_layerPopQueue)
	{
		// Check for active layer change.
		if (layer == hoveredLayer) app.m_hoveredLayer = nullptr;
		if (layer == focusedLayer)
		{
			// Check if design engine was removed.
			// TODO: Thas to be a better way to handle this.
			auto* engineLayer = dynamic_cast<EngineLayer<Design2DEngine>*>(focusedLayer);
			if (engineLayer)
				app.m_guiState->design_engine = nullptr;

			// Reset.
			app.m_focusedLayer = nullptr;
			ImGui::SetWindowFocus(NULL);
		}
		// Remove layer.
		m_layers.erase(layer->getName());
	}

	// All the layers have been removed.
	m_layerPopQueue.clear();
	m_layerPopQueue.reserve(m_layers.size()-2);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//