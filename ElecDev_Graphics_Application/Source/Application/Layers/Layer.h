#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <integer.hpp>
#include "External/ImGUI/Core/imgui.h"
#include "External/ImGUI/Core/imgui_internal.h"
#include <memory>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

class Layer
{
public:

	// Constructor.
	Layer(std::string layerName);

	// Pass an event to the layer.
	virtual void onEvent(Event& event) = 0;

	// Render the specific layer.
	virtual void onRender() = 0;

	// Update the data related to the layer.
	// This function has to be called after a ImGui element has begun.
	// The layer will use the data related to the specific ImGui element.
	// If the layer consists of only on window it is simple, since the layer
	// and the window uses the same data.  Uses the data from the window contained
	// in the class.
	virtual void dispatchEvents(); 

	// Set the name of all the elements in the layer.
	inline virtual void setName(std::string& newName) = 0;

	// --------------- //
	//  G E T T E R S  //
	// --------------- //
	
	// Checks if the layer is docked.
	bool isDocked();
	// Get the ImGui dock ID.
	ImGuiID getDockID();
	// Get the name of the layer.
	std::string getName();
	// Get the ImGui window associated with the layer.
	// Should return a vector if the layer has more than one window.
	bool isHovered();

protected:

	friend class LayerStack;

	// Set the docking state.
	void setDockingState(bool state);
	// Handle the docking state change.
	// This will change where the layer sits
	// in the stack.
	void onDockingStateChange(bool newState);
	// Check if the layer has been resized.
	void detectResize();
	// Check if the layer has been moved.
	void detectMove();

	// --------- //
	//  D A T A  //
	// --------- //

	// The name of the layer.  This is required since
	// the window names in ImGUI acts like ID's.
	std::string m_layerName = "NULL";
	// The size of the layer.
	ImVec2 m_layerSize = { 0.f, 0.f };
	// The position of the layer.  Also used as the minimum value in the border.
	ImVec2 m_layerPosition = { 0.f, 0.f };
	// Is the layer docked?
	bool m_docked = false;
	// The ImGui dock ID related to the window.
	ImGuiID m_dockID = 0;
	// Pointer to the ImGui window.
	ImGuiWindow* m_imGuiWindow = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
