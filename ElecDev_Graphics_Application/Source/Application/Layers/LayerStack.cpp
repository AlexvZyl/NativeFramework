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

std::vector<std::unique_ptr<Layer>>& LayerStack::getLayers()
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
	{
		// Find layer.
		auto it = std::find(m_layers.begin(), m_layers.end(), *layer);

		// If layer is found, erase.
		if (it != m_layers.end())
		{
			int index = it - m_layers.begin();
			m_layers.erase(m_layers.begin() + index);
		}
		// Log warning.
		else std::cout << yellow << "\n[LAYERS] [WARN]: " << white << "Tried to remove a layer that is not on the stack.\n";
	}
	// All the layers have been removed.
	m_layers.shrink_to_fit();
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
