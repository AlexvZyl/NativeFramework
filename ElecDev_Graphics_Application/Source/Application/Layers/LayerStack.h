#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>
#include <unordered_map>
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
	// It returns the new name of the layer with its ID appended.
	template<class LayerType>
	std::string pushLayer(std::unique_ptr<LayerType>& layer);

	// Queue the layer to be removed.
	void queuePopLayer(Layer& layer);
	// Pop all the layers that have been queued for removal.
	// We need to pass the layers in use to check if they have been removed.
	void popLayers();
	// The above two functions are implemented in this way, because we dont want to 
	// immediately remove a layer while it is dispatching events or rendering.
	// It should be done in the following frame.

	// Get the layers contained in the stack.
	std::unordered_map<std::string, std::unique_ptr<Layer>>& getLayers();

	// Retrieve the layer from the dictionary.
	template<class LayerType>
	LayerType* getLayer(std::string& layerName);

private:

	// Map containing all of the layers.
	std::unordered_map<std::string, std::unique_ptr<Layer>> m_layers;
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
std::string LayerStack::pushLayer(std::unique_ptr<LayerType>& layer)
{
	// Create a name with an unqiue ID.
	// This allows us to have windows with the same name.
	layer->setID(m_totalLayerCount);
	m_totalLayerCount++;
	// Resets the name with the ID.
	layer->setName(layer->getName());
	// We want to return the new name with the ID.
	std::string newName = layer->getName();

	// Push the layer.
	m_layers.insert({ layer->getName(), std::move(layer) });

	// Resize the layer pop queue.
	m_layerPopQueue.reserve(m_layers.size());

	// Return the name with the ID.
	return newName;
}

template<class LayerType>
LayerType* LayerStack::getLayer(std::string& layerName)
{
	return dynamic_cast<LayerType*>(m_layers[layerName].get());
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//