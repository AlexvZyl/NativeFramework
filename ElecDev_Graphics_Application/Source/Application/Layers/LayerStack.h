#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include <vector>
#include <memory>
#include "Application/Layers/Layer.h"
#include <string>

//==============================================================================================================================================//
//  Layer Stack.																																//
//==============================================================================================================================================//

class LayerStack 
{
public:

	// Constructor.
	LayerStack() = default;

	// Add a layer to the front of the stack.
	template<typename LayerType>
	void pushLayer(Layer& layer);

	// Add the specified layer to the to be removed vector.
	void queuePopLayer(Layer& layer);
	// Pop all the layers that have been scheduled to be removed.
	void popLayers();
	// The above two functions are implemented in this way, because we dont want to 
	// immediately remove a layer while it is dispatching events or rendering.
	// It should be done in the following frame.

	// Get the layers in the stack.
	std::vector<std::unique_ptr<Layer>>& getLayers();

private:

	// Vector containing all of the layers.
	std::vector<std::unique_ptr<Layer>> m_layers;
	// Layers that are queued for removal.
	std::vector<Layer*> m_layerPopQueue;

	// Count of the total layers that have passed through the stack.
	// Just used to create a unique ID for the layers.
	unsigned m_totalLayerCount = 0;

};

//==============================================================================================================================================//
//  Operators.																																	//
//==============================================================================================================================================//

bool operator==(const std::unique_ptr<Layer>& layerUPtr, const Layer& layer);
bool operator!=(const std::unique_ptr<Layer>& layerUPtr, const Layer& layer);

//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template<typename LayerType>
void LayerStack::pushLayer(Layer& layer)
{
	// Create a unique imgui ID that is not displayed so that we can have
	// windows with the same name.
	std::string newName = layer.m_layerName + "##" + std::to_string(m_totalLayerCount);
	m_totalLayerCount++;
	layer.changeName(newName);
	// Push the layer.
	m_layers.emplace_back(std::make_unique<LayerType>(std::move(dynamic_cast<LayerType&>(layer))));
	m_layerPopQueue.reserve(m_layers.size());
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//