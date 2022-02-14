#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <memory>
#include "Application/Layers/Layer.h"
#include "External/Misc/ConsoleColor.h"

//==============================================================================================================================================//
//  Layer Stack.																																//
//==============================================================================================================================================//

class LayerStack 
{
public:

	// Constructor.
	LayerStack() = default;

	// Add a layer to the stack.
	template<class LayerType>
	std::string pushLayer(Layer& layer);

	// Queue the layer to be removed.
	void queuePopLayer(Layer& layer);
	// Pop all the layers that have been queued for removal.
	void popLayers();
	// The above two functions are implemented in this way, because we dont want to 
	// immediately remove a layer while it is dispatching events or rendering.
	// It should be done in the following frame.

	// Get the layers contained in the stack.
	std::map<std::string, std::unique_ptr<Layer>>& getLayers();

	// Retrieve the layer from the dictionary.
	template<class LayerType>
	LayerType* getLayer(std::string& layerName) 
	{
		return dynamic_cast<LayerType*>(m_layers[layerName].get());
	}

private:

	// Vector containing all of the layers.
	std::map<std::string, std::unique_ptr<Layer>> m_layers;
	// Layers that are queued for removal.
	std::vector<Layer*> m_layerPopQueue;

	// Count of the total layers that have passed through the stack.
	// Just used to create a unique ID for the layers.
	unsigned m_totalLayerCount = 0;

};

//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template<class LayerType>
std::string LayerStack::pushLayer(Layer& layer)
{
	// Create a name with an unqiue ID.
	// This allows us to have windows with the same name.
	std::string newName = layer.getName() + "##" + std::to_string(m_totalLayerCount);
	layer.setName(newName);
	m_totalLayerCount++;

	// Push the layer.
	m_layers.insert({ layer.getName(), std::make_unique<LayerType>(std::move(dynamic_cast<LayerType&>(layer))) });

	// Resize the layer pop queue.
	m_layerPopQueue.reserve(m_layers.size());

	// return the name with the ID.
	return newName;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//