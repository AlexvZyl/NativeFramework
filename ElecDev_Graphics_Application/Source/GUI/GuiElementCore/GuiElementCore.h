#pragma once

//==============================================================================================================================================//
//  Includes..																																	//
//==============================================================================================================================================//

#include <string>
#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;
class WindowEvent;
class MouseButtonEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyEvent;
class NotifyEvent;
class Layer;

struct ImGuiWindow;

//==============================================================================================================================================//
//  GUI Core Element.																															//
//==============================================================================================================================================//

class GuiElementCore 
{
public:

	// Pass event to element.
	virtual void onEvent(Event& event);

	// Constructor.
	GuiElementCore(std::string name, int windowFlags);
	// Destructor.
	inline virtual ~GuiElementCore() = default;

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Start the ImGUI widget.
	inline virtual void begin() = 0;
	// Render the ImGUI widget.
	inline virtual void onRender() = 0;
	// End the ImGUI widget.
	inline virtual void end() = 0;
	// Updates the data related to the gui element.
	virtual void onUpdate();
	// Should the gui element render, based on certain flags.
	bool shouldRender();
	// Update the render state.
	void updateRenderState();
	// Event handler called when the render state changes.
	inline virtual void onRenderStateChange(bool newState) {};

	// --------- //
	//  D A T A  //
	// --------- //

	// Should the window close?
	bool m_isOpen = true;
	// Is the window collapsed?
	bool m_isCollapsed = false;
	// Is the window hidden?
	bool m_isHidden = false;
	// Is the GUI element docked?
	bool m_isDocked = false;
	// Should the element be rendered?
	bool m_shouldRender = true;
	// The gui dock id.
	ImGuiID m_dockID = NULL;
	// The window name.
	std::string m_name = "NULL";
	// The ImGUI flags describing the window.
	int m_imguiWindowFlags = 0;
	// The imgui window that the gui is in.
	ImGuiWindow* m_imguiWindow = nullptr;
	// The size of the content area.
	glm::vec2 m_contentRegionSize = { 0.f, 0.f };
	// The position of the content area.
	glm::vec2 m_contentRegionPosition = { 0.f, 0.f };

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Checks if the window is hovered.
	bool isHovered();

	// Mouse events.
	inline virtual void onMouseButtonEvent(MouseButtonEvent& event) {};
	inline virtual void onMouseMoveEvent(MouseMoveEvent& event) {};
	inline virtual void onMouseScrollEvent(MouseScrollEvent& event) {};

	// Key events.
	inline virtual void onKeyEvent(KeyEvent& event) {};

	// Content region events.
	void detectContentRegionResize();
	void detectContentRegionMove();
	virtual void onContentRegionResizeEvent(WindowEvent& event);
	virtual void onContentRegionMoveEvent(WindowEvent& event);

	// Layer events.
	virtual void onNotifyEvent(NotifyEvent& event);
	inline virtual void onFocusEvent(NotifyEvent& event) {};
	inline virtual void onDefocusEvent(NotifyEvent& event) {};
	inline virtual void onHoverEvent(NotifyEvent& event) {};
	inline virtual void onDehoverEvent(NotifyEvent& event) {};

	// Return the mouse position in the local scene coordinates (pixels).
	// (0,0) is in the top left.
	glm::vec2 getMouseLocalPosition();
	glm::vec2 getMouseGlobalPosition();

	// The layer that the GUI belongs to.
	Layer* m_layer;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
