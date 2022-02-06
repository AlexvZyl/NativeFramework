//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "Application/Events/Events.h"
#include "Application/Layers/Layer.h"
#include "External/Misc/ConsoleColor.h"
#include "Engines/EngineCore/EngineCore.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "GUI/ComponentEditor/ComponentEditor.h"

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

Layer::Layer(uint64_t ID, std::string layerName) 
	: m_layerID(ID), m_layerName(layerName)
{}

bool Layer::isMouseHovering()
{
	return ImGui::IsMouseHoveringRect(m_layerPosition, m_borderMax, false);		// We should check how the clipping works in ImGui.
}

//==============================================================================================================================================//
//  Layer data.																																	//
//==============================================================================================================================================//

void Layer::updateLayerData()
{
	detectResize();
	detectMove();
	setDockingState(ImGui::IsWindowDocked());
}

void Layer::setDockingState(bool state)
{
	// On state change.
	if (m_docked != state)
	{
		onDockingStateChange(state);
		m_docked = state;
	}
}

void Layer::onDockingStateChange(bool newState) 
{
	// If docked, get the dock ID.
	if (newState) m_dockID = ImGui::GetWindowDockID();
	// Log the dock.
	std::cout << m_layerName << ": Docking state is " << newState << ".\n";
	
	// The layer should be moved (in the stack) based on where it has been docked/undocked.
	// It should go to the layers with which it was docked.
}

void Layer::detectResize() 
{
	ImVec2 currentSize = ImGui::GetWindowSize();
	// Check for resize.
	if (currentSize.x != m_layerSize.x || currentSize.y != m_layerSize.y)
	{
		// Create resize event to pass onto layer elements.
		glm::vec2 eventSize = { currentSize.x, currentSize.y };
		WindowResizeEvent event(eventSize, 0);
		onEvent(event);
		// Update layer size.
		m_layerSize = currentSize;
		// Update the layer border.
		m_borderMax = { m_layerPosition.x + m_layerSize.x, m_layerPosition.y + m_layerSize.y };
	}
}

void Layer::detectMove() 
{
	ImVec2 currentPos = ImGui::GetWindowPos();
	// Check for move.
	if (currentPos.x != m_layerPosition.x || currentPos.y != m_layerPosition.y)
	{
		// Update the layer position.
		m_layerPosition = currentPos;
		// Update the layer border.
		m_borderMax = { m_layerPosition.x + m_layerSize.x, m_layerPosition.y + m_layerSize.y };
	}
}

//==============================================================================================================================================//
//  Setters and getters.																														//
//==============================================================================================================================================//

uint64_t Layer::getLayerID()
{
	return m_layerID;
}

ImGuiID Layer::getDockID()
{
	return m_dockID;
}

std::string& Layer::getLayerName() 
{
	return m_layerName;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
