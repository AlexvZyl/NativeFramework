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
#include "GUI/ImGuiTweaks.h"

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

Layer::Layer(uint64_t ID, std::string layerName) 
	: m_layerID(ID), m_layerName(layerName)
{}

//==============================================================================================================================================//
//  Layer data.																																	//
//==============================================================================================================================================//

void Layer::dispatchLayerEvents()
{
	// Ensure that the window has been created.
	if (!m_imGuiWindow) return;
	
	// Detect dock state change.
	setDockingState(m_imGuiWindow->DockIsActive);
	 
	// Currently we have no need for this information to be updated.
	//// Check for layer resizes.
	//detectLayerResize();
	//detectLayerMove();
}

void Layer::setDockingState(bool state) 
{
	if (m_docked != state) onDockingStateChange(state);
}

void Layer::onDockingStateChange(bool newState) 
{
	// Change state.
	m_docked = newState;
	// If docked, get the dock ID.
	if (newState) m_dockID = ImGui::GetWindowDockID();
}

void Layer::detectLayerResize() 
{
	ImVec2 currentSize = m_imGuiWindow->Size;
	// Check for resize.
	if (currentSize.x != m_layerSize.x || currentSize.y != m_layerSize.y) 
		m_layerSize = currentSize;
}

void Layer::detectLayerMove() 
{
	ImVec2 currentPos = m_imGuiWindow->Pos;
	if (currentPos.x != m_layerPosition.x || currentPos.y != m_layerPosition.y) 
		m_layerPosition = currentPos;
}

//==============================================================================================================================================//
//  Setters and getters.																														//
//==============================================================================================================================================//

bool Layer::isLayerHovered()
{
	if(!m_imGuiWindow) m_imGuiWindow = ImGui::FindWindowByName(m_layerName.c_str());
	return ImGuiTweaks::IsWindowHovered(0, m_imGuiWindow);
}

uint64_t Layer::getLayerID()
{
	return m_layerID;
}

ImGuiID Layer::getDockID()
{
	return m_dockID;
}

std::string Layer::getLayerName() 
{
	return m_layerName;
}

bool Layer::isDocked() 
{
	return m_docked;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
