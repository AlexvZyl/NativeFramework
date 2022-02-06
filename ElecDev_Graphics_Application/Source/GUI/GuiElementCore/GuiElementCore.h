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
class WindowResizeEvent;

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

	// Resize event.
	virtual void onWindowResizeEvent(WindowResizeEvent& event);

	// Is the window open?
	bool m_isOpen = true;
	// The window name.
	std::string m_name;
	// The ImGUI flags describing the window.
	int m_imguiWindowFlags = 0;

	// Start the ImGUI widget.
	virtual void begin();
	// Render the ImGUI widget.
	virtual void renderBody();
	// End the ImGUI widget.
	virtual void end();

	// The size of the gui component.
	ImVec2 m_size;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
