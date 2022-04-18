//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/EventLog.h"
#include "Application/LumenWindow/LumenWindow.h"
#include "Application/LumenWindow/WindowStack.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/ComponentDesigner.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Utilities/Profiler/Profiler.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  window event dispatching.																													//
//==============================================================================================================================================//

void Application::onUpdate()
{
	LUMEN_PROFILE_SCOPE("App OnUpdate");

	// Execute the Lua scripts.
	executeLuaScriptQueue();

	// Log messages.
	Logger::flushQueue();

	// Pop the windows queued from the previous frame's rendering.
	// Dispatched here so that they do not get GLFW events.
	popWindows();

	// Find the hovered window on a mouse move event.
	if (EventLog::mouseMoveOccured())
	{
		// If there is no hovered window, we need to check if a window is hovered.
		if (!m_hoveredWindow) 
			onHoveredWindowChange(findHoveredWindow());

		// If the currently hovered window is no longer being hovered, we need to find the new window.
		else if(!m_hoveredWindow->isHovered()) 
			onHoveredWindowChange(findHoveredWindow());
	}
	
	// Dispatch GLFW events.
	for (auto& event : EventLog::getEvents())
	{
		// Application events are dispatched explicitly (since it is not a part of the windows).
		if (event->isType(EventType_Application))
		{
			Application::onEvent(*event.get()); 
			continue;
		}
		// File drop should go to the hovered window.
		else if (event->isType(EventType_FileDrop) && m_hoveredWindow) 
		{
			m_hoveredWindow->onEvent(*event.get());
			continue;
		}
		
		// On a mouse press we need to change the focused window.
		// This also allows us to modify how dear imgui sets focused windows.
		if (event->isType(EventType_MousePress))
			onFocusedWindowChange(m_hoveredWindow); 

		// Pass events to focused window.
		if (m_focusedWindow) 
			m_focusedWindow->onEvent(*event.get());
	}

	// These mouse events are kept seperate to prevent handling events more than once per frame.
	if (m_hoveredWindow)
	{
		if (EventLog::mouseMoveOccured())
			m_hoveredWindow->onEvent(EventLog::getMouseMove());

		if (EventLog::mouseScrollOccured()) 
			m_hoveredWindow->onEvent(EventLog::getMouseScroll());

		if (EventLog::mouseDragOccured())
			m_hoveredWindow->onEvent(EventLog::getMouseDrag());
	}

	// Dispatch notify events after all of the other events are done.
	if (m_focusedWindow)
	{
		for (auto& event : EventLog::getNotifyEvents())
		{
			m_focusedWindow->onEvent(event);
		}
	}

	// Pop windows that are queued from GLFW events.
	// We pop them here so that they do not get dispatched for onUpdate.
	popWindows();

	// Dispatch the events that are handled by the windows.
	// These include things such as window resizes and docking state changes.
	// They are events that occur and GLFW is not aware of, since windows essentially 
	// are their own windows.  Currently every window is checked every frame.  This is not 
	// necessary.  The only thing preventing us from only updating only the focused window is 
	// due to how resizing works when windows are docked.  They do not necessarily
	// come into focus, missing the resize event.
	for (auto& [ID, window] : m_windowStack->getWindows())
		window->onUpdate();

	// All of the GLFW events have been handled and the log can be cleared.
	EventLog::clear();
}

LumenWindow* Application::findHoveredWindow()
{
	// Find the window that is being hovered.
	// We do not have to worry about order, since dear imgui handles it.
	for (auto& [ID, window] : m_windowStack->getWindows())
	{
		if (window->isHovered())
			return window.get();
	}
	// No window is found.
	return nullptr;
}

void Application::imguiOnUpdate() 
{
	LUMEN_PROFILE_SCOPE("ImGui OnUpdate");

	// Pass events to ImGui.
	// Thess events are called here so that ImGui is not hammered with more that one per frame.
	if (EventLog::mouseScrollOccured())
	{
		MouseScrollEvent& event = EventLog::getMouseScroll();
		ImGui_ImplGlfw_ScrollCallback(getGLFWWindow(), event.xOffset, event.yOffset);
	}
	if (EventLog::mouseMoveOccured())
	{
		MouseMoveEvent& event = EventLog::getMouseMove();
		ImGui_ImplGlfw_CursorPosCallback(getGLFWWindow(), event.mousePosition.x, event.mousePosition.y);
	}
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

void Application::onHoveredWindowChange(LumenWindow* newWindow)
{
	// Ensure change actually ocurred.
	if(newWindow == m_hoveredWindow) return;

	// Create a dehover event.
	if (m_hoveredWindow)
	{
		NotifyEvent dehoverEvent(EventType_Dehover);
		m_hoveredWindow->onEvent(dehoverEvent);
	}

	// Create a hover event.
	if (newWindow)
	{
		NotifyEvent hoverEvent(EventType_Hover);
		newWindow->onEvent(hoverEvent);
	}

	// Set the new hovered window.
	m_hoveredWindow = newWindow;
}

void Application::onFocusedWindowChange(LumenWindow* newWindow)
{
	// Ensure change actually ocurred.
	if (newWindow == m_focusedWindow) return;

	// Let ImGui set focus in this case.
	if (!newWindow && ImGui::GetItemID()) return;

	// Create a defocus event.
	if (m_focusedWindow)
	{
		m_focusedWindow->onEvent(NotifyEvent(EventType_Defocus));
	}

	// Create a focus event.
	if (newWindow)
	{
		newWindow->onEvent(NotifyEvent(EventType_Focus));
		newWindow->focus();
	}
	// No window is being hovered.
	else ImGui::SetWindowFocus(NULL);

	// Assign new focused window.
	m_focusedWindow = newWindow;
}

//==============================================================================================================================================//
//  Application events.																															//
//==============================================================================================================================================//

void Application::onEvent(Event& event)
{
	// Window events.																	 
	if      (event.isType(EventType_WindowResize))	{ onWindowResizeEvent(dynamic_cast<WindowEvent&>(event)); }
												 								 
	// File events.					 								 
	else if (event.isType(EventType_FileDrop))		{ onFileDropEvent(dynamic_cast<FileDropEvent&>(event)); }
	else if (event.isType(EventType_FileSave))		{ onFileSaveEvent(dynamic_cast<FileSaveEvent&>(event)); }
	else if (event.isType(EventType_FileLoad))		{ onFileLoadEvent(dynamic_cast<FileLoadEvent&>(event)); }

	// Event unhandled.
	else LUMEN_LOG_WARN("No handler for event.", "Application");
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

void Application::onWindowResizeEvent(WindowEvent& event)
{
	// This should pass a scaled window resize event to all of the windows.  I think...
}

//==============================================================================================================================================//
//  File events.																																//
//==============================================================================================================================================//

void Application::onFileDropEvent(FileDropEvent& event)
{
	// Load all of the paths in the event.
	for (auto& path : event.fileData)
	{
		if (path.string().size())
			loadFromYAML(path);
	}
}

void Application::onFileSaveEvent(FileSaveEvent& event) 
{
	// Iterate through the paths.
	for (auto& path : event.fileData)
	{
		// Check if operation did not fail.
		if (path.string().size())
		{
			Design2DEngine* designEngine = event.getEngine<Design2DEngine>();
			ComponentDesigner* component_designer = event.getEngine<ComponentDesigner>();

			if (designEngine) 
			{
				saveToYAML(designEngine->m_circuit.get(), path);
				designEngine->setName(path.filename().stem().string());
			}
			else if (component_designer) 
			{
				saveToYAML(component_designer->m_activeComponent.get(), path);
				component_designer->setName(path.filename().stem().string());
			}
		}
	}
}

void Application::onFileLoadEvent(FileLoadEvent& event)
{
	// Load all of the paths in the event.
	for (auto& path : event.fileData)
	{
		if (path.string().size())
			loadFromYAML(path);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

