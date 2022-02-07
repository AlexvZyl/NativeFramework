//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Events/Events.h"
#include "External/ImGUI/Core/imgui.h"

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

GuiElementCore::GuiElementCore(std::string name, int windowFlags)
	: m_name(name), m_imguiWindowFlags(windowFlags)
{}

void GuiElementCore::begin() 
{
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void GuiElementCore::renderBody() 
{
	// Override this function to render the window.
}

void GuiElementCore::end()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  On Event.																																	//
//==============================================================================================================================================//

void GuiElementCore::onEvent(Event& event)
{
	// Check if already consumed.
	if (event.isConsumed()) return;

	uint64_t eventID = event.getID();

	// Mouse events.
	if		(eventID == EventType_MouseMove)	{ onMouseMoveEvent(dynamic_cast<MouseMoveEvent&>(event)); }
	else if (eventID == EventType_MouseScroll)	{ onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(event)); }
	else if (eventID == EventType_MousePress)	{ onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }
	else if (eventID == EventType_MouseRelease) { onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }

	// Key events.
	else if (eventID == EventType_KeyPress)		{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }
	else if (eventID == EventType_KeyRelease)	{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }

	// Window events.
	else if (eventID == EventType_WindowResize) { onWindowResizeEvent(dynamic_cast<WindowEvent&>(event)); }

	// Layer events.
	else if (eventID == EventType_Focus)		{ onFocusEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Defocus)		{ onDefocusEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Hover)		{ onHoverEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Dehover)		{ onDehoverEvent(dynamic_cast<LayerEvent&>(event)); }

	// Gui should not consume the event.
}

//==============================================================================================================================================//
//  Window Events.																																//
//==============================================================================================================================================//

void GuiElementCore::onWindowResizeEvent(WindowEvent& event)
{
	glm::vec2 windowSize = event.getWindowData();
	m_contentRegionSize = { windowSize.x, windowSize.y };
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//