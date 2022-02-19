//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/EventLog.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"

//==============================================================================================================================================//
//  Layer event dispatching.																													//
//==============================================================================================================================================//

void Application::dispatchEvents()
{
	// Find the hovered layer on a mouse move event.
	if (m_eventLog->mouseMove)
	{
		// If there is no hovered layer, we need to check if a layer is hovered.
		if (!m_hoveredLayer) onHoveredLayerChange(findhoveredLayer());

		// If the currently hovered layer is no longer being hovered we need to find the new layer.
		else if(!m_hoveredLayer->isHovered()) onHoveredLayerChange(findhoveredLayer());
	}
	
	// Dispatch general events.
	for (std::unique_ptr<Event>& event : m_eventLog->events)
	{
		uint64_t eventID = event->ID;

		// Application specific are dispatched explicitly (since it is not a part of the layers).
		if (eventID == EventType_Application)
		{
			Application::onEvent(*event.get()); 
			continue;
		}
		
		// On a mouse press we need to change the focused layer.
		// This also allows us to modify how dear imgui sets focused layers.
		if (eventID == EventType_MousePress) 
			onFocusedLayerChange(m_hoveredLayer); 

		// Pass events to focused layer.
		if (m_focusedLayer) 
			m_focusedLayer->onEvent(*event.get());
	}

	// These mouse events are kept seperate to prevent handling events more than once per frame.
	if (m_hoveredLayer)
	{
		if (m_eventLog->mouseMove)   m_hoveredLayer->onEvent(*m_eventLog->mouseMove.get());
		if (m_eventLog->mouseScroll) m_hoveredLayer->onEvent(*m_eventLog->mouseScroll.get());
	}

	// Some layers are queued to pop after events.
	m_layerStack->popLayers();

	// Dispatch the events that are handled by the layers.
	// These include things such as window resizes and docking state changes.
	// They are events that occur and GLFW is not aware of, since layers essentially 
	// are their own windows.  Currently every layer is checked every frame.  This is not 
	// necessary.  The only thing preventing us from only updating only the focused layer is 
	// due to how resizing works when windows are docked.  They do not necessarily
	// come into focus, missing the resize event.
	for (auto& [name, layer] : m_layerStack->getLayers())
		layer->dispatchEvents();

	// All of the GLFW events have been handled and the log can be cleared.
	m_eventLog->clear();
}

Layer* Application::findhoveredLayer() 
{
	// Find the layer that is being hovered.
	// We do not have to worry about order, since dear imgui handles it.
	// This could be optimized by ordering the layer (finding the
	// layer will happen faster) but we will always have very few layers.
	for (auto& [name, layer] : m_layerStack->getLayers())
	{
		if (layer->isHovered())
			return layer.get();
	}
	// No layer is found.
	return nullptr;
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
		LayerEvent dehoverEvent(EventType_Dehover);
		m_hoveredLayer->onEvent(dehoverEvent);
	}

	// Create a hover event.
	if (newLayer)
	{
		LayerEvent hoverEvent(EventType_Hover);
		newLayer->onEvent(hoverEvent);
	}

	// Set the new hovered layer.
	m_hoveredLayer = newLayer;
}

void Application::onFocusedLayerChange(Layer* newLayer)
{
	// Ensure change actually ocurred.
	if (newLayer == m_focusedLayer) return;

	// Create a defocus event.
	if (m_focusedLayer)
	{
		LayerEvent defocusEvent(EventType_Defocus);
		m_focusedLayer->onEvent(defocusEvent);
	}

	// Create a focus event.
	if (newLayer)
	{
		LayerEvent focusEvent(EventType_Focus);
		newLayer->onEvent(focusEvent);
		newLayer->focus();
	}
	// No layer is in focus.
	else ImGui::SetWindowFocus(NULL);

	// Assign new focused layer.
	m_focusedLayer = newLayer;
}

//==============================================================================================================================================//
//  Application events.																															//
//==============================================================================================================================================//

void Application::onEvent(Event& event)
{
	
	uint64_t eventID = event.ID;
	
	// Window events.																	 
	if      (eventID == EventType_WindowResize)	{ onWindowResizeEvent(dynamic_cast<WindowEvent&>(event)); }
												 								 
	// File events.					 								 
	else if (eventID == EventType_FileDrop)		{ onFileDropEvent(dynamic_cast<FileEvent&>(event)); }
	else if (eventID == EventType_FileSave)		{ onFileSaveEvent(dynamic_cast<FileEvent&>(event)); }
	else if (eventID == EventType_FileLoad)		{ onFileLoadEvent(dynamic_cast<FileEvent&>(event)); }
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

void Application::onWindowResizeEvent(WindowEvent& event)
{
	// This should pass a scaled window resize event to all of the layers.
}

//==============================================================================================================================================//
//  File events.																																//
//==============================================================================================================================================//

void Application::onFileDropEvent(FileEvent& event)
{

}

void Application::onFileSaveEvent(FileEvent& event) 
{
	//// Check if operation did not fail.
	//if (m_loadEvent.path != "OPERATION_CANCELLED" && m_loadEvent.path != "FOLDER_EMPTY")
	//{
	//	// Find engine.
	//	Design2DEngineGL* saveEngine = reinterpret_cast<Design2DEngineGL*>(m_windowsDictionary[m_saveEvent.saveEngine]->engineGL.get());

	//	// Check if file is added to the save event.
	//	std::string savePath = m_saveEvent.path;
	//	if (savePath.find(".lmct") != std::string::npos ||
	//		savePath.find(".yml") != std::string::npos ||
	//		savePath.find(".yaml") != std::string::npos)
	//	{
	//		// Move the file onto a new string.
	//		std::string file;
	//		while (savePath.back() != '\\')
	//		{
	//			file.push_back(savePath.back());
	//			savePath.pop_back();
	//		}
	//		std::reverse(file.begin(), file.end());
	//		saveToYAML(saveEngine->m_circuit, savePath, file);
	//	}
	//	else
	//	{
	//		saveToYAML(saveEngine->m_circuit, m_saveEvent.path);
	//	}
	//}
	//m_saveEvent.saveEngine = "";
	//m_saveEvent.eventTrigger = false;
	//m_saveEvent.path = "";
}

void Application::onFileLoadEvent(FileEvent& event)
{
	// Load all of the paths in the event.
	for (auto& path : event.fileData)
	{
		// Check if operation did not fail.
		// This should be handled differently!
		if (path != "OPERATION_CANCELLED" && path != "FOLDER_EMPTY")
		{
			// Load file into Lumen.
			loadFromYAML(path);
		}
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//

