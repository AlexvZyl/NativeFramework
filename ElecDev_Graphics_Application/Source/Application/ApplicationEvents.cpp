//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/EventLog.h"
#include "Events/EventLog.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"

//==============================================================================================================================================//
//  Layer event dispatching.																													//
//==============================================================================================================================================//

void Application::dispatchEvents()
{
	// Check for hovered layer if there is none.
	if (!m_hoveredLayer) onHoveredLayerChange(findhoveredLayer());
	
	// If current hovered layer is no longer being hovered.
	else if (!m_hoveredLayer->isMouseHovering()) onHoveredLayerChange(findhoveredLayer());

	// Dispatch the events.
	for (std::unique_ptr<Event>& event : m_eventLog->events)
	{
		uint64_t eventID = event->getID();

		// Application specific events (since it is not a part of the layers).
		if (eventID == EventType_Application)
		{
			Application::onEvent(*event.get()); 
			continue;
		}
		
		// On a mouse press we need to change the focused layer.
		if (eventID == EventType_MousePress) 
		{ 
			// Only change focus if the layers are different.
			if (m_hoveredLayer != m_focusedLayer) onFocusedLayerChange(m_hoveredLayer); 
		}

		// Pass events to focused layer.
		if (m_focusedLayer) m_focusedLayer->onEvent(*event.get());
	}

	// These mouse events are kept seperately to prevent handling events more than once.
	if (m_hoveredLayer)
	{
		if (m_eventLog->mouseMove)   m_hoveredLayer->onEvent(*m_eventLog->mouseMove.get());
		if (m_eventLog->mouseScroll) m_hoveredLayer->onEvent(*m_eventLog->mouseScroll.get());
	}

	// All events have been handled.
	m_eventLog->clear();
}

Layer* Application::findhoveredLayer() 
{
	// Find the layer that is being hovered.  Iterate in 
	// reverse since the layers last added are in front.
	std::vector<std::unique_ptr<Layer>>& layers = m_layerStack->getLayers();
	bool layerFound = false;
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		if (layers[i]->isMouseHovering())
			return layers[i].get();
	}
	// No layer is found.
	return nullptr;
}

//==============================================================================================================================================//
//  Layer events.																																//
//==============================================================================================================================================//

void Application::onHoveredLayerChange(Layer* newLayer)
{
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
		ImGui::SetWindowFocus(newLayer->getLayerName().c_str());
	}
	else 
	{
		ImGui::SetWindowFocus(NULL);
	}

	// Assign new focused layer.
	m_focusedLayer = newLayer;
}

//==============================================================================================================================================//
//  Application events.																															//
//==============================================================================================================================================//

void Application::onEvent(Event& event)
{
	// Return if event is consumed.
	if (event.isConsumed()) return;

	uint64_t eventID = event.getID();
	
	// Window events.																	 
	if      (eventID == EventType_WindowResize)	{ onWindowResizeEvent(dynamic_cast<WindowResizeEvent&>(event)); }
												 								 
	// Serialisation events.					 								 
	else if (eventID == EventType_FileDrop)		{ onFileDropEvent(dynamic_cast<FileDropEvent&>(event)); }

	// Consume event.
	event.consume();
}

void Application::onWindowResizeEvent(WindowResizeEvent& event)
{

}

void Application::onFileDropEvent(FileDropEvent& event)
{

}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
