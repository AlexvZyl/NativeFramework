//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <integer.hpp>
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Application/Layers/LayerStack.h"
#include "Application/Layers/Layer.h"
#include "Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Layer management.																															//
//==============================================================================================================================================//

std::map<std::string, std::unique_ptr<Layer>>& LayerStack::getLayers()
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

	// Get active layers.
	Layer* hoveredLayer = Lumen::getApp().m_hoveredLayer;
	Layer* focusedLayer = Lumen::getApp().m_focusedLayer;

	// Remove layers.
	for (Layer* layer : m_layerPopQueue)
	{
		// Check for active layer change.
		if (layer == hoveredLayer) Lumen::getApp().onHoveredLayerChange(nullptr);
		if (layer == focusedLayer) Lumen::getApp().onFocusedLayerChange(nullptr);
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