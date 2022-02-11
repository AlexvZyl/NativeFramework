//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Events/Events.h"
#include "External/ImGUI/Core/imgui.h"
#include "External/ImGUI/Core/imgui_internal.h"

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

GuiElementCore::GuiElementCore(std::string name, int windowFlags)
	: m_name(name), m_imguiWindowFlags(windowFlags)
{}

bool GuiElementCore::shouldRender() 
{
	return !m_isCollapsed && !m_isHidden;
}

//==============================================================================================================================================//
//  Event dispatching.																															//
//==============================================================================================================================================//

void GuiElementCore::onEvent(Event& event)
{
	// Log the event.
	std::cout << m_name << ": " << event.ID << "\n";

	uint64_t eventID = event.ID;

	// Layer events should be passed even if the gui is collapsed.
	if		(eventID == EventType_Focus)		{ onFocusEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Defocus)		{ onDefocusEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Hover)		{ onHoverEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Dehover)		{ onDehoverEvent(dynamic_cast<LayerEvent&>(event)); }

	// Do not pass the events below if the layer is collapsed.
	else if (!shouldRender()) return;

	// Mouse events.
	else if	(eventID == EventType_MouseMove)	{ onMouseMoveEvent(dynamic_cast<MouseMoveEvent&>(event)); }
	else if (eventID == EventType_MouseScroll)	{ onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(event)); }
	else if (eventID == EventType_MousePress)	{ onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }
	else if (eventID == EventType_MouseRelease) { onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }

	// Key events.
	else if (eventID == EventType_KeyPress)		{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }
	else if (eventID == EventType_KeyRelease)	{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }

	// Window events.
	else if (eventID == EventType_WindowResize) { onContentRegionResizeEvent(dynamic_cast<WindowEvent&>(event)); }
	else if (eventID == EventType_WindowMove)	{ onContentRegionMoveEvent(dynamic_cast<WindowEvent&>(event)); }
}

void GuiElementCore::dispatchGuiEvents(ImGuiWindow* window)
{
	// Update data.
	m_isCollapsed = window->Collapsed;
	m_isDocked = window->DockIsActive;
	m_isHidden = window->Hidden;

	// Check if we should pass the events.
	if (!shouldRender()) return;

	// Update gui data.
	detectContentRegionMove(window);
	detectContentRegionResize(window);
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void GuiElementCore::onContentRegionResizeEvent(WindowEvent& event)
{
	m_contentRegionSize		= { event.windowData.x, event.windowData.y };
}

void GuiElementCore::onContentRegionMoveEvent(WindowEvent& event)
{
	m_contentRegionPosition	= { event.windowData.x, event.windowData.y };
}

void GuiElementCore::detectContentRegionResize(ImGuiWindow* window) 
{
	// Get the current content region size.
	ImVec2 contentRegionSizeIm = window->ContentRegionRect.GetSize();
	glm::vec2 contentRegionSize = { contentRegionSizeIm.x, contentRegionSizeIm.y};

	// If the content region resized pass an event.
	if (m_contentRegionSize.x != contentRegionSize.x || m_contentRegionSize.y != contentRegionSize.y)
	{
		WindowEvent event(contentRegionSize, EventType_WindowResize);
		onEvent(event);	
	}
}

void GuiElementCore::detectContentRegionMove(ImGuiWindow* window)
{
	// Get window current position.
	ImVec2 regionPos = window->ContentRegionRect.Min;
	glm::vec2 contentRegionPos = { regionPos.x, regionPos.y };

	// Check if the window has moved.
	if(m_contentRegionPosition.x != regionPos.x || m_contentRegionPosition.y != regionPos.y)
	{
		WindowEvent event(contentRegionPos, EventType_WindowMove);
		onEvent(event);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//