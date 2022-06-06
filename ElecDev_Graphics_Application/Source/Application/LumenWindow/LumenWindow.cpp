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
	WIDTH_OF_SPACE = ImGui::CalcTextSize(" ").x;
}

void LumenWindow::onRender()
{
	onImGuiBegin();

	updateRenderStateFlags();

	if (shouldRender()) onImGuiRender();

	onImGuiEnd();

	if (!m_isOpen) closeWindow();
}

void LumenWindow::closeWindow()
{
	Lumen::getApp().queueWindowPop(this);
}

void LumenWindow::onUpdate()
{
	if (!findImGuiWindow()) return;

	updateRenderStateFlags();
	m_dockID = m_imguiWindow->DockId;

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
	if (!m_imguiWindow) return false;
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_DockHierarchy, m_imguiWindow);
}

ImGuiWindow* LumenWindow::findImGuiWindow()
{
	if (m_imguiWindow) return m_imguiWindow;
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
	float currentWidth = ImGui::CalcTextSize(m_windowName.c_str()).x;

	// Wider than min width.
	if (currentWidth > MIN_TAB_WIDTH)
	{
		m_imguiName = m_windowName + "###LumenWindow" + std::to_string(m_lumenWindowID);
		return;
	}

	// Need to add white space so that it is at least the minimum width.
	// TODO: This leaves an inconsistent min width.
	int spacesToAdd = std::ceil((MIN_TAB_WIDTH - currentWidth)/WIDTH_OF_SPACE);
	std::string spaces = "";
	for (int i = 0; i < spacesToAdd; i++) spaces.push_back(' ');
	m_imguiName = m_windowName + spaces + "###LumenWindow" + std::to_string(m_lumenWindowID);
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

void LumenWindow::dockWindow(LumenWindow* window, ImGuiDir direction) 
{
	// Dock into the same node.
	if (direction == ImGuiDir_None)
	{
		ImGui::DockBuilderDockWindow(window->getImGuiName(), m_dockID);
	}
	// Split the node and dock into new node.
	else 
	{
		ImGuiID newNode = ImGui::DockBuilderSplitNode(m_dockID, direction, 0.4f, NULL, &m_dockID);
		ImGui::DockBuilderDockWindow(window->getImGuiName(), newNode);
	}
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
	return globalToLocalCoords({ cursorX, getMainViewportSize().y - cursorY });
}

glm::vec2 LumenWindow::getMouseGlobalPosition() const
{
	double cursorX, cursorY;
	glfwGetCursorPos(Lumen::getApp().getGLFWWindow(), &cursorX, &cursorY);
	return { cursorX, cursorY };
}

glm::vec2 LumenWindow::globalToLocalCoords(const glm::vec2& coords) const 
{
	return { coords.x - m_contentRegionPosition.x,
			 coords.y - (getMainViewportSize().y - (m_contentRegionPosition.y + m_contentRegionSize.y)), };
}

glm::vec2 LumenWindow::localToGlobalCoords(const glm::vec2& coords) const
{
	return { coords.x + m_contentRegionPosition.x,
			 coords.y + (getMainViewportSize().y - (m_contentRegionPosition.y + m_contentRegionSize.y)), };
}

glm::vec2 LumenWindow::getMainViewportSize() const 
{
	return Lumen::getApp().getMainViewportSize();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//