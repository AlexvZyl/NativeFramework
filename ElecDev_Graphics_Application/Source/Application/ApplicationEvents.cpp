//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/EventLog.h"
#include "Application/Windows/LumenWindow.h"
#include "Application/Windows/WindowStack.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/ComponentDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Utilities/Profiler/Profiler.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  window event dispatching.																													//
//==============================================================================================================================================//

void Application::onUpdate()
{
	LUMEN_PROFILE_SCOPE("App OnUpdate");

	// Update subsystems.
	resizeWindows();
	executeLuaScriptQueue();
	Logger::flushQueue();
	//setGuiTheme();

	// Dispatch the events that are handled by the windows.
	// These include things such as window resizes and docking state changes.
	// They are events that occur and GLFW is not aware of, since windows essentially 
	// are their own windows.  This has to be done before GLFW events are dispatched to the 
	// windows since some elements relie on some of these states being updated.
	for (auto& [ID, window] : m_windowStack->getWindows())
		window->onUpdate();

	// Find the hovered window on a mouse move event.
	if (EventLog::mouseMoveOccurred())
	{
		// If there is no hovered window, we need to check if a window is hovered.
		if (!m_hoveredWindow) onHoveredWindowChange(findHoveredWindow());

		// If the currently hovered window is no longer being hovered, we need to find the new window.
		else if (!m_hoveredWindow->isHovered()) onHoveredWindowChange(findHoveredWindow());
	}
	
	// Dispatch GLFW events.
	for (auto& event : EventLog::getEvents())
	{
		// Resize windows on a left release.
		if (event->isType(EventType_MouseRelease))
		{
			startWindowResizing();
		}

		// Application events are dispatched explicitly (since it is not a part of the windows).
		if (event->isType(EventType_Application))
		{
			Application::onEvent(*event.get()); 
			continue;
		}

		// File drop should go to the hovered window.
		else if (m_hoveredWindow && event->isType(EventType_FileDrop))
		{
			m_hoveredWindow->onEvent(*event.get());
			continue;
		}
		
		// On a mouse press we need to change the focused window.
		// This also allows us to modify how dear imgui sets focused windows.
		else if (event->isType(EventType_MousePress))
		{
			// Change focus if clicked on a new window.
			if (m_hoveredWindow && m_hoveredWindow != m_focusedWindow) m_hoveredWindow->focus();
			// Double check if a focus should occur.  For now this is a workaround where tabs are not setting focus.
			else onFocusedWindowChange(findFocusedWindow());
			// If there is a window after the change, pass the event.
			if (m_focusedWindow) m_focusedWindow->onEvent(*event.get());
		}

		// Pass save events to the active engine.
		else if (event->isType(EventType_FileSave))
		{
			getActiveEngine()->onEvent(*event.get());
		}

		// Pass events to focused window.
		else if (m_focusedWindow)
		{
			m_focusedWindow->onEvent(*event.get());
		}
	}

	// These mouse events are kept seperate to prevent handling events more than once per frame.
	if (m_hoveredWindow)
	{
		if (EventLog::mouseScrollOccurred()) m_hoveredWindow->onEvent(EventLog::getMouseScroll());
		if (EventLog::mouseDragOccurred())	 m_hoveredWindow->onEvent(EventLog::getMouseDrag());
		if (EventLog::mouseMoveOccurred())   m_hoveredWindow->onEvent(EventLog::getMouseMove());
	}

	// Dispatch notify events after all of the other events are done.
	if (m_focusedWindow)
	{
		for (auto& event : EventLog::getNotifyEvents()) 
			m_focusedWindow->onEvent(event);
	}

	// Pop windows that are queued from GLFW events.
	// We pop them here so that they do not get dispatched for onUpdate.
	popWindows();

	// All of the GLFW events have been handled and the log can be cleared.
	EventLog::clear();
}

void Application::imguiOnUpdate() 
{
	LUMEN_PROFILE_SCOPE("ImGui OnUpdate");

	// Pass events to ImGui.
	// Thess events are called here so that ImGui is not hammered with more that one per frame.
	if (EventLog::mouseScrollOccurred())
	{
		MouseScrollEvent& event = EventLog::getMouseScroll();
		ImGui_ImplGlfw_ScrollCallback(getGLFWWindow(), event.xOffset, event.yOffset);
	}
	if (EventLog::mouseMoveOccurred())
	{
		MouseMoveEvent& event = EventLog::getMouseMove();
		ImGui_ImplGlfw_CursorPosCallback(getGLFWWindow(), event.mousePosition.x, getMainViewportSize().y - event.mousePosition.y);
	}
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

void Application::onHoveredWindowChange(LumenWindow* newWindow)
{
	if(newWindow == m_hoveredWindow) return;

	if (m_hoveredWindow) m_hoveredWindow->onEvent(NotifyEvent(EventType_Dehover));
	if (newWindow)		 newWindow->onEvent(NotifyEvent(EventType_Hover));

	m_hoveredWindow = newWindow;
}

void Application::onFocusedWindowChange(LumenWindow* newWindow)
{
	if (newWindow == m_focusedWindow) return;

	if (m_focusedWindow)  m_focusedWindow->onEvent(NotifyEvent(EventType_Defocus));
	if (newWindow)		  newWindow->onEvent(NotifyEvent(EventType_Focus));

	m_focusedWindow = newWindow;
}

//==============================================================================================================================================//
//  Application events.																															//
//==============================================================================================================================================//

void Application::onEvent(const Event& event)
{
	// Window events.																	 
	if      (event.isType(EventType_WindowResize))	{ onWindowResizeEvent(event.cast<WindowEvent>()); }
												 								 
	// File events.					 								 
	else if (event.isType(EventType_FileDrop))		{ onFileDropEvent(event.cast<FileDropEvent>()); }
	else if (event.isType(EventType_FileLoad))		{ onFileLoadEvent(event.cast<FileLoadEvent>()); }

	// Event unhandled.
	else LUMEN_LOG_WARN("No handler for event.", "Application");
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

void Application::onWindowResizeEvent(const WindowEvent& event)
{
	// This should pass a scaled window resize event to all of the windows.  I think...
	// This could help solve the multiple resolutions problem?

	startWindowResizing();
}

//==============================================================================================================================================//
//  File events.																																//
//==============================================================================================================================================//

void Application::onFileDropEvent(const FileDropEvent& event)
{
	// Load all of the paths in the event.
	for (auto& path : event.fileData)
	{
		if (path.string().size()) loadFromYAML(path);
	}
}

void Application::onFileLoadEvent(const FileLoadEvent& event)
{
	// Load all of the paths in the event.
	for (auto& path : event.fileData)
	{
		if (path.string().size()) loadFromYAML(path);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

