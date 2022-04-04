//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Events/Events.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Application/Application.h"
#include "Lumen.h"
#include "Utilities/Profiler/Profiler.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

GuiElementCore::GuiElementCore(std::string name, int windowFlags)
	: m_name(name), m_imguiWindowFlags(windowFlags)
{}

bool GuiElementCore::shouldRender() 
{
	return m_shouldRender;
}

void GuiElementCore::updateRenderState() 
{
	bool newState = !m_isCollapsed && !m_isHidden && m_isOpen;
	// If the state has changed.
	if (m_shouldRender != newState)
	{
		onRenderStateChange(newState);
		m_shouldRender = newState;
	}
}

//==============================================================================================================================================//
//  Event dispatching.																															//
//==============================================================================================================================================//

void GuiElementCore::onEvent(Event& event)
{
	if (event.isConsumed()) return;

	uint64_t eventID = event.ID;

	// Notify events.
	if		(eventID == EventType_Notify)		{ onNotifyEvent(dynamic_cast<NotifyEvent&>(event)); }
	
	// Window events.
	else if (eventID == EventType_WindowResize) { onContentRegionResizeEvent(dynamic_cast<WindowEvent&>(event)); }
	else if (eventID == EventType_WindowMove)	{ onContentRegionMoveEvent(dynamic_cast<WindowEvent&>(event)); }

	// Do not pass the events below if the layer is not to be rendered.
	else if (!shouldRender()) return;

	// Mouse events.
	else if	(eventID == EventType_MouseMove)	{ onMouseMoveEvent(dynamic_cast<MouseMoveEvent&>(event)); }
	else if (eventID == EventType_MouseScroll)	{ onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(event)); }
	else if (eventID == EventType_MousePress)	{ onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }
	else if (eventID == EventType_MouseRelease) { onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }

	// Key events.
	else if (eventID == EventType_KeyPress)		{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }
	else if (eventID == EventType_KeyRelease)	{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }
}

void GuiElementCore::onUpdate()
{
	// Get window if it does not exist.
	if (!m_imguiWindow)
	{
		m_imguiWindow = ImGui::FindWindowByName(m_name.c_str());
		// If it does still not exist, return.
		if (!m_imguiWindow) return;
	}

	// Update data.
	m_isCollapsed	= m_imguiWindow->Collapsed;
	m_isDocked		= m_imguiWindow->DockIsActive;
	m_isHidden		= m_imguiWindow->Hidden;
	updateRenderState();

	// Get dock ID if it has not been assigned.
	if (!m_dockID) m_dockID = m_imguiWindow->DockId;

	// If it should not be rendered we do not have to 
	// check for resizes.
	if (!shouldRender()) return;

	// Update gui data.
	detectContentRegionMove();
	detectContentRegionResize();
}

void GuiElementCore::onNotifyEvent(NotifyEvent& event)
{
	uint64_t eventID = event.ID;
	if		(eventID == EventType_Focus)	{ onFocusEvent(event); }
	else if (eventID == EventType_Defocus)	{ onDefocusEvent(event); }
	else if (eventID == EventType_Hover)	{ onHoverEvent(event); }
	else if (eventID == EventType_Dehover)	{ onDehoverEvent(event); }
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

bool GuiElementCore::isHovered()
{
	// If the window exists.
	if (m_imguiWindow)
		return ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_DockHierarchy, m_imguiWindow);

	// Window does not exist yet.
	return false;
}

void GuiElementCore::onContentRegionResizeEvent(WindowEvent& event)
{
	m_contentRegionSize		= { event.windowData.x, event.windowData.y };
}

void GuiElementCore::onContentRegionMoveEvent(WindowEvent& event)
{
	m_contentRegionPosition	= { event.windowData.x, event.windowData.y };
}

void GuiElementCore::detectContentRegionResize() 
{
	// Get the current content region size.
	glm::vec2 contentRegionSize = m_imguiWindow->WorkRect.GetSize();
	// If the content region resized pass an event.
	if (m_contentRegionSize.x != contentRegionSize.x || m_contentRegionSize.y != contentRegionSize.y)
	{
		WindowEvent event(contentRegionSize, EventType_WindowResize);
		onEvent(event);	
	}
}

void GuiElementCore::detectContentRegionMove()
{
	// Get window current position.
	glm::vec2 contentRegionPos = m_imguiWindow->WorkRect.Min;
	// Check if the window has moved.
	if(m_contentRegionPosition.x != contentRegionPos.x || m_contentRegionPosition.y != contentRegionPos.y)
	{
		WindowEvent event(contentRegionPos, EventType_WindowMove);
		onEvent(event);
	}
}

glm::vec2 GuiElementCore::getMouseLocalPosition() 
{
	// Get the cursor position.
	double cursorX, cursorY;
	glfwGetCursorPos(Lumen::getApp().getWindow(), &cursorX, &cursorY);
	return { cursorX - m_contentRegionPosition.x, cursorY - m_contentRegionPosition.y };
}

glm::vec2 GuiElementCore::getMouseGlobalPosition()
{
	// Get the cursor position.
	double cursorX, cursorY;
	glfwGetCursorPos(Lumen::getApp().getWindow(), &cursorX, &cursorY);
	return { cursorX, cursorY };
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//