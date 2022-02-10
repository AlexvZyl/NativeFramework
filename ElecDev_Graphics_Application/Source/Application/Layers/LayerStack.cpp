//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <integer.hpp>
#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Application/Layers/LayerStack.h"
#include "Application/Layers/Layer.h"

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

	// Remove layers.
	for (Layer* layer : m_layerPopQueue)
		m_layers.erase(layer->getLayerName());

	// All the layers have been removed.
	m_layerPopQueue.clear();
	m_layerPopQueue.reserve(m_layers.size());
}

//==============================================================================================================================================//
//  Operators.																																	//
//==============================================================================================================================================//

bool operator==(const std::unique_ptr<Layer>& layerUPtr, const Layer& layer)
{
	return layerUPtr.get() == &layer;
}

bool operator!=(const std::unique_ptr<Layer>& layerUPtr, const Layer& layer) 
{
	return not (layerUPtr.get() == &layer);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
