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

class EngineCore;
class GuiElementCore;
class Event;
class GraphicsScene;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

enum LayerType
{
	LayerType_Undefined			=	1 << 0,
	LayerType_App				=	1 << 1,

	// GUI elements.
	LayerType_GUI				=	1 << 2,
	LayerType_Ribbon			=	1 >> 3,
	LayerType_Toolbar			=	1 >> 4,
	LayerType_PopUpMenu			=	1 >> 5,
	LayerType_GraphicsScene		=	1 >> 6,
	LayerType_CircuitEditor		=	1 >> 7,
	LayerType_ComponentEditor	=	1 >> 8,

	// Engines.
	LayerType_Engine			=	1 << 9,
	LayerType_Base2DEngine		=	1 << 10,
	LayerType_Base3DEngine		=	1 << 11,
	LayerType_Design2DEngine	=	1 << 12,
	LayerType_Design3DEngine	=	1 << 13,
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

	// Get the layer ID.
	uint64_t getID();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) = 0;

	// Render the specific layer.
	virtual void onRender() = 0;

protected:

	// The name of the layer.  This is required since
	// the window names in ImGUI acts like ID's.
	std::string m_layerName;

	// Set if the layer is docked.
	void setDockingState(bool state);

	// Check if the element has been resized.
	virtual void detectResize();

	// Set the layer border for the layer.
	void setBorder(ImVec2& min, ImVec2& max);

	// Handle the docking state change.
	// This will change where the layer sits
	// in the stack.
	void onDockingStateChange(bool newState);

	// Update the data related to the layer.
	void updateLayerData();

	// Descirption of the layer.
	uint64_t m_layerID = 0;
	// Bounding box min coordinate.
	ImVec2 m_borderMin = { 0.f, 0.f };
	// Bounding box max coordinate.
	ImVec2 m_borderMax = { 0.f, 0.f };
	// The size of the layer.
	ImVec2 m_layerSize = { 0.f, 0.f };
	// Is the layer docked?
	bool m_docked = false;
	// The ImGui dock ID related to the window.
	ImGuiID m_dockID = 0;
};

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

class GuiLayer : public Layer
{
public:

	// Create a GUI layer based on the ID.
	GuiLayer(uint64_t, std::string layerName, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GuiElementCore* getGuiElement();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the specific layer.
	virtual void onRender() override;

protected:

	// The GUI element that belongs to this layer.
	std::unique_ptr<GuiElementCore> m_guiElement = nullptr;

	// The imguiWindow flags related to the later.
	int m_imguiWindowflags = 0;

};

//==============================================================================================================================================//
//  Engine Layer.																																//
//==============================================================================================================================================//

class EngineLayer : public Layer
{
public:

	// Create an engine layer based on the ID.
	EngineLayer(uint64_t ID, std::string layerName, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GuiElementCore* getGuiElement();

	// Get the engine in the layer.
	EngineCore* getEngine();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the specific layer.
	virtual void onRender() override;

private:

	// The engine the the layer belongs to.
	std::unique_ptr<EngineCore> m_engine = nullptr;

	// The GUI element that belongs to this layer.
	std::unique_ptr<GraphicsScene> m_guiElement = nullptr;

	// The imguiWindow flags related to the later.
	int m_imguiWindowflags = 0;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
