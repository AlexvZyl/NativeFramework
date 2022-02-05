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
}

void LayerStack::popLayer(Layer& layer)
{
	//// Find layer.
	//auto it = std::find(m_layers.begin(), m_layers.end(), std::make_unique<Layer>(layer));

	//// If layer is found, erase.
	//if (it != m_layers.end())
	//{
	//	int index = it - m_layers.begin();
	//	m_layers.erase(m_layers.begin() + index);
	//}
	//// Log warning.
	//else
	//{
	//	std::cout << yellow << "\n[LAYERS] [WARN]: " << white << "Tried to remove a layer that is not on the stack.\n";
	//}
}

std::vector<std::unique_ptr<Layer>>& LayerStack::getLayers()
{
	return m_layers;
}


//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//