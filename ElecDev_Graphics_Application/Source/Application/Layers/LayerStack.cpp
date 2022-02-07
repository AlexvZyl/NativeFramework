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

void LayerStack::popTopLayer()
{
	m_layers.pop_back();
	m_layers.reserve(m_layers.size() - 1);
}

std::vector<std::unique_ptr<Layer>>& LayerStack::getLayers()
{
	return m_layers;
}

void LayerStack::popLayer(Layer& layer)
{
	// Find layer.
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);

	// If layer is found, erase.
	if (it != m_layers.end())
	{
		int index = it - m_layers.begin();
		m_layers.erase(m_layers.begin() + index);
		m_layers.reserve(m_layers.size() - 1);
	}

	// Log warning.
	else std::cout << yellow << "\n[LAYERS] [WARN]: " << white << "Tried to remove a layer that is not on the stack.\n";
}

void LayerStack::moveLayerToFront(Layer& layer) 
{
	// Find layer.
	auto it = std::find(m_layers.begin(), m_layers.end(), layer);

	// Do nothing is the layer is already in front.
	if (layer == m_layers.back()) return;

	// If layer is found, move to front.
	if (it != m_layers.end())
	{
		int index = it - m_layers.begin();
		m_layers.emplace_back(std::move(m_layers[index]));
		m_layers.erase(m_layers.begin() + index);
	}

	// Log warning.
	else std::cout << yellow << "\n[LAYERS] [WARN]: " << white << "Tried to move a layer that is not on the stack.\n";
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