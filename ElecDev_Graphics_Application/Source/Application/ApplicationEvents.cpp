//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/EventLog.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/ComponentDesigner.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Utilities/Profiler/Profiler.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Layer event dispatching.																													//
//==============================================================================================================================================//

void Application::onUpdate()
{
	LUMEN_PROFILE_SCOPE("App OnUpdate");

	//Application::setGuiTheme();

	// Execute the Lua scripts.
	executeLuaScriptQueue();

	// Log messages.
	Logger::flushQueue();

	// Pop the layers queued from the previous frame's rendering.
	// Dispatched here so that they do not get GLFW events.
	popLayers();

	// Find the hovered layer on a mouse move event.
	if (EventLog::mouseMove)
	{
		// If there is no hovered layer, we need to check if a layer is hovered.
		if (!m_hoveredLayer) 
			onHoveredLayerChange(findHoveredLayer());

		// If the currently hovered layer is no longer being hovered, we need to find the new layer.
		else if(!m_hoveredLayer->isHovered()) 
			onHoveredLayerChange(findHoveredLayer());
	}
	
	// Dispatch GLFW events.
	for (auto& event : EventLog::events)
	{
		// Application events are dispatched explicitly (since it is not a part of the layers).
		if (event->isType(EventType_Application))
		{
			Application::onEvent(*event.get()); 
			continue;
		}
		
		// On a mouse press we need to change the focused layer.
		// This also allows us to modify how dear imgui sets focused layers.
		if (event->isType(EventType_MousePress))
			onFocusedLayerChange(m_hoveredLayer); 

		// Pass events to focused layer.
		if (m_focusedLayer) 
			m_focusedLayer->onEvent(*event.get());
	}

	// These mouse events are kept seperate to prevent handling events more than once per frame.
	if (m_hoveredLayer)
	{
		if (EventLog::mouseMove)
			m_hoveredLayer->onEvent(*EventLog::mouseMove.get());

		if (EventLog::mouseScroll) 
			m_hoveredLayer->onEvent(*EventLog::mouseScroll.get());

		if (EventLog::mouseDrag)
			m_hoveredLayer->onEvent(*EventLog::mouseDrag.get());
	}

	// Pop layers that are queued from GLFW events.
	// We pop them here so that they do not get dispatched for onUpdate.
	popLayers();

	// Dispatch the events that are handled by the layers.
	// These include things such as window resizes and docking state changes.
	// They are events that occur and GLFW is not aware of, since layers essentially 
	// are their own windows.  Currently every layer is checked every frame.  This is not 
	// necessary.  The only thing preventing us from only updating only the focused layer is 
	// due to how resizing works when windows are docked.  They do not necessarily
	// come into focus, missing the resize event.
	for (auto& [name, layer] : m_layerStack->getLayers())
		layer->onUpdate();

	// All of the GLFW events have been handled and the log can be cleared.
	EventLog::clear();
}

Layer* Application::findHoveredLayer()
{
	// Find the layer that is being hovered.
	// We do not have to worry about order, since dear imgui handles it.
	for (auto& [name, layer] : m_layerStack->getLayers())
	{
		if (layer->isHovered())
			return layer.get();
	}
	// No layer is found.
	return nullptr;
}

void Application::imguiOnUpdate() 
{
	LUMEN_PROFILE_SCOPE("ImGui OnUpdate");

	// Pass events to ImGui.
	// Thess events are called here so that ImGui is not hammered with more that one per frame.
	if (EventLog::mouseScroll)
		ImGui_ImplGlfw_ScrollCallback(m_window, EventLog::mouseScroll->xOffset, EventLog::mouseScroll->yOffset);
	if (EventLog::mouseMove)
		ImGui_ImplGlfw_CursorPosCallback(m_window, EventLog::mouseMove->mousePosition.x, EventLog::mouseMove->mousePosition.y);
}

//==============================================================================================================================================//
//  Layer events.																																//
//==============================================================================================================================================//

void Application::onHoveredLayerChange(Layer* newLayer)
{
	// Ensure change actually ocurred.
	if(newLayer == m_hoveredLayer) return;

	// Create a dehover event.
	if (m_hoveredLayer)
	{
		NotifyEvent dehoverEvent(EventType_Dehover);
		m_hoveredLayer->onEvent(dehoverEvent);
	}

	// Create a hover event.
	if (newLayer)
	{
		NotifyEvent hoverEvent(EventType_Hover);
		newLayer->onEvent(hoverEvent);
	}

	// Set the new hovered layer.
	m_hoveredLayer = newLayer;
}

void Application::onFocusedLayerChange(Layer* newLayer)
{
	// Ensure change actually ocurred.
	if (newLayer == m_focusedLayer) return;

	// Let ImGui set focus in this case.
	if (!newLayer && ImGui::GetItemID()) return;

	// Create a defocus event.
	if (m_focusedLayer)
	{
		NotifyEvent defocusEvent(EventType_Defocus);
		m_focusedLayer->onEvent(defocusEvent);
	}

	// Create a focus event.
	if (newLayer)
	{
		NotifyEvent focusEvent(EventType_Focus);
		newLayer->onEvent(focusEvent);
		newLayer->focus();
	}
	// No layer is being hovered.
	else ImGui::SetWindowFocus(NULL);

	// Assign new focused layer.
	m_focusedLayer = newLayer;
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
	// This should pass a scaled window resize event to all of the layers.  I think...
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
			loadFromYAML(path.string());
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
				saveToYAML(designEngine->m_circuit, path);
			}
			else if (component_designer) 
			{
				saveToYAML(component_designer->m_activeComponent, path);
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
			loadFromYAML(path.string());
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

