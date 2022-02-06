#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <integer.hpp>
#include "External/ImGUI/Core/imgui.h"
#include <memory>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

enum LayerType
{
	// Basic GUI elements.
	LayerType_GUI				=	1 << 0,
	LayerType_Ribbon			=	1 >> 1,
	LayerType_Toolbar			=	1 >> 2,
	LayerType_PopUpMenu			=	1 >> 3,
	LayerType_GraphicsScene		=	1 >> 4,
	LayerType_CircuitEditor		=	1 >> 5,
	LayerType_ComponentEditor	=	1 >> 6,

	// Engines.
	LayerType_Engine			=	1 << 7,
	LayerType_Base2DEngine		=	1 << 8,
	LayerType_Base3DEngine		=	1 << 9,
	LayerType_Design2DEngine	=	1 << 10,
	LayerType_Design3DEngine	=	1 << 11,
};

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

class Layer
{
public:

	// Constructor.
	Layer(uint64_t ID, std::string layerName);

	// Checks if the mouse is hovering the layer.
	bool isMouseHovering();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) = 0;

	// Render the specific layer.
	virtual void onRender() = 0;

	// Get the layer ID.
	uint64_t getLayerID();
	// Get the ImGui dock ID.
	ImGuiID getDockID();
	// Get the name of the layer.
	std::string& getLayerName();

protected:

	// The name of the layer.  This is required since
	// the window names in ImGUI acts like ID's.
	std::string m_layerName;

	// Set if the layer is docked.
	void setDockingState(bool state);

	// Check if the layer has been resized.
	virtual void detectResize();

	// Check if the layer has been moved.
	virtual void detectMove();

	// Handle the docking state change.
	// This will change where the layer sits
	// in the stack.
	void onDockingStateChange(bool newState);

	// Update the data related to the layer.
	// This function has to be called after a ImGui element has begun.
	// The layer will use the data related to the specific ImGui element.
	// If the layer consists of only on window it is simple, since the layer
	// and the window uses the same data.  Just call it after ImGui::Begin().
	// If a layer is going to contain more than one window we will probably have
	// to either use the docking, or ImGui::BeginChild().
	void updateLayerData();

	// Descirption of the layer.
	uint64_t m_layerID = 0;
	// Bounding box max coordinate.
	ImVec2 m_borderMax = { 0.f, 0.f };
	// The size of the layer.
	ImVec2 m_layerSize = { 0.f, 0.f };
	// The position of the layer.  Also used as the minimum value in the border.
	ImVec2 m_layerPosition = { 0.f, 0.f };
	// Is the layer docked?
	bool m_docked = false;
	// The ImGui dock ID related to the window.
	ImGuiID m_dockID = 0;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
