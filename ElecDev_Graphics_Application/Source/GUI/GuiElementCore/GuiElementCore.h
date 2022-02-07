#pragma once

//==============================================================================================================================================//
//  Includes..																																	//
//==============================================================================================================================================//

#include <string>
#include "External/ImGUI/Core/imgui.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;
class WindowEvent;
class MouseButtonEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyEvent;
class LayerEvent;

//==============================================================================================================================================//
//  GUI Core Element.																															//
//==============================================================================================================================================//

class GuiElementCore 
{
public:

	// Pass event to element.
	virtual void onEvent(Event& event);

protected:

	friend class Layer;
	friend class BasicGuiLayer;
	friend class EngineLayer;

	// Constructor.
	GuiElementCore(std::string name, int windowFlags);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Start the ImGUI widget.
	virtual void begin();
	// Render the ImGUI widget.
	virtual void renderBody();
	// End the ImGUI widget.
	virtual void end();

	// --------- //
	//  D A T A  //
	// --------- //

	// Is the window open?
	bool m_isOpen = true;
	// The window name.
	std::string m_name = "NULL";
	// The ImGUI flags describing the window.
	int m_imguiWindowFlags = 0;

	// The size of the content area.
	ImVec2 m_contentRegionSize = { 0.f, 0.f };
	// The position of the content area.
	ImVec2 m_contentRegionPosition = { 0.f, 0.f };

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Mouse events.
	inline virtual void onMouseButtonEvent(MouseButtonEvent& event) {};
	inline virtual void onMouseMoveEvent(MouseMoveEvent& event) {};
	inline virtual void onMouseScrollEvent(MouseScrollEvent& event) {};

	// Key events.
	inline virtual void onKeyEvent(KeyEvent& event) {};

	// Window events.
	virtual void onWindowResizeEvent(WindowEvent& event);
	virtual void onWindowMoveEvent(WindowEvent& event);

	// Layer events.
	inline virtual void onFocusEvent(LayerEvent& event) {};
	inline virtual void onDefocusEvent(LayerEvent& event) {};
	inline virtual void onHoverEvent(LayerEvent& event) {};
	inline virtual void onDehoverEvent(LayerEvent& event) {};
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
