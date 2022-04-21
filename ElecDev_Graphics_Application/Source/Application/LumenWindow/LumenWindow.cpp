#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "LumenWindow.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Application/Events/Events.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Lumen Window.																																//
//==============================================================================================================================================//

void LumenWindow::onAttach(unsigned ID)
{
	m_lumenWindowID = ID;
	updateImGuiName();
}

void LumenWindow::onRender()
{
	onImGuiBegin();

	updateRenderStateFlags();

	if (shouldRender())
		onImGuiRender();

	onImGuiEnd();

	if (!m_isOpen)
		closeWindow();
}

void LumenWindow::closeWindow()
{
	Lumen::getApp().queueWindowPop(this);
}

void LumenWindow::onUpdate()
{
	// If no window is found it should not be updated.
	if (!findImGuiWindow()) return;

	updateRenderStateFlags();

	// Do not check for events if it is not being rendererd.
	if (!shouldRender()) return;

	detectWindowMove();
	detectWindowResize();
}

//==============================================================================================================================================//
//  ImGui API.																																	//
//==============================================================================================================================================//

int LumenWindow::getImGuiWindowFlags() const
{
	return m_imguiWindowFlags;
}

void LumenWindow::setImGuiWindowFlags(int flags) 
{
	m_imguiWindowFlags = flags;
}

void LumenWindow::addImGuiWindowFlags(int flags)
{
	m_imguiWindowFlags |= flags;
}

void LumenWindow::removeImGuiWindowFlags(int flags)
{
	m_imguiWindowFlags &= ~flags;
}

bool LumenWindow::isHovered() const
{
	if (!m_imguiWindow) return false; // No window currently exists.
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_DockHierarchy, m_imguiWindow);
}

ImGuiWindow* LumenWindow::findImGuiWindow()
{
	if (m_imguiWindow) return m_imguiWindow;  // Window already found.
	m_imguiWindow = ImGui::FindWindowByName(getImGuiName());
	return m_imguiWindow;
}

void LumenWindow::focus()
{
	ImGui::SetWindowFocus(getImGuiName());
	Lumen::getApp().onFocusedWindowChange(this);
}

void LumenWindow::unsavedDocument()
{
	addImGuiWindowFlags(ImGuiWindowFlags_UnsavedDocument);
}

void LumenWindow::savedDocument()
{
	removeImGuiWindowFlags(ImGuiWindowFlags_UnsavedDocument);
}

const char* LumenWindow::getImGuiName() const
{
	return m_imguiName.c_str();
}

void LumenWindow::updateImGuiName()
{
	m_imguiName = m_windowName + "###LumenWindow" + std::to_string(m_lumenWindowID);
}

bool LumenWindow::shouldRender() const
{
	return m_shouldRender;
}

void LumenWindow::updateRenderStateFlags()
{
	if (!m_imguiWindow) return;
	m_isCollapsed = m_imguiWindow->Collapsed;
	m_isDocked = m_imguiWindow->DockIsActive;
	m_isHidden = m_imguiWindow->Hidden;
	m_skipItems = m_imguiWindow->SkipItems;
	detectRenderStateChange();
}

void LumenWindow::detectRenderStateChange()
{
	// Check for shouldRender state change.
	bool newState = !m_isCollapsed && !m_isHidden && !m_skipItems && m_isOpen;
	if (m_shouldRender != newState)
	{
		onRenderStateChange(newState);
		m_shouldRender = newState;
	}
}

const glm::vec2& LumenWindow::getContentRegionSize() const 
{
	return m_contentRegionSize;
}

//==============================================================================================================================================//
//  Window ID.																																	//
//==============================================================================================================================================//

const std::string& LumenWindow::getName() const
{
	return m_windowName;
}

void LumenWindow::setName(const char* name)
{
	m_windowName = name;
	updateImGuiName();
}

unsigned LumenWindow::getID() const
{
	return m_lumenWindowID;
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void LumenWindow::onWindowResizeEvent(const WindowEvent& event)
{
	m_contentRegionSize = { event.windowData.x, event.windowData.y };
}

void LumenWindow::onWindowMoveEvent(const WindowEvent& event)
{
	m_contentRegionPosition = { event.windowData.x, event.windowData.y };
}

void LumenWindow::detectWindowResize()
{
	glm::vec2 contentRegionSize = m_imguiWindow->WorkRect.GetSize();
	if (m_contentRegionSize.x != contentRegionSize.x || m_contentRegionSize.y != contentRegionSize.y)
	{
		onWindowResizeEvent(WindowEvent(contentRegionSize, EventType_WindowResize));
	}
}

void LumenWindow::detectWindowMove()
{
	glm::vec2 contentRegionPos = m_imguiWindow->WorkRect.Min;
	if (m_contentRegionPosition.x != contentRegionPos.x || m_contentRegionPosition.y != contentRegionPos.y)
	{
		onWindowMoveEvent(WindowEvent(contentRegionPos, EventType_WindowMove));
	}
}

glm::vec2 LumenWindow::getMouseLocalPosition() const
{
	double cursorX, cursorY;
	glfwGetCursorPos(Lumen::getApp().getGLFWWindow(), &cursorX, &cursorY);
	return { cursorX - m_contentRegionPosition.x, cursorY - m_contentRegionPosition.y };
}

glm::vec2 LumenWindow::getMouseGlobalPosition() const
{
	double cursorX, cursorY;
	glfwGetCursorPos(Lumen::getApp().getGLFWWindow(), &cursorX, &cursorY);
	return { cursorX, cursorY };
}

glm::vec2 LumenWindow::globalToLocalCoords(const glm::vec2& coords) 
{
	return { coords.x - m_contentRegionPosition.x,
			 coords.y - m_contentRegionPosition.y, };
}

glm::vec2 LumenWindow::localToGlobalCoords(const glm::vec2& coords)
{
	return { coords.x + m_contentRegionPosition.x,
			 coords.y + m_contentRegionPosition.y, };
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//