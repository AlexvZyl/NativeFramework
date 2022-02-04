//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/EventLog.h"
#include "Graphics/graphicsHandler.h"
#include "Events/EventLog.h"

//==============================================================================================================================================//
//  Layer dispatching.																															//
//==============================================================================================================================================//

void Application::dispatchEvents()
{
	// Handle all of the events in the list.
	for (std::unique_ptr<Event>& event : eventLog.events)
	{
		uint64_t layer = event->getLayer();
		if (layer == EventLayer::App)		{ onEvent(*event.get()); }
		if (layer == EventLayer::GUI)		{						 }
		if (layer == EventLayer::Engine)	{						 }
	}

	// Handle the mouse move event.
	uint64_t layer = eventLog.mouseMove->getLayer();
	if (layer == EventLayer::App)		{ onEvent(*eventLog.mouseMove.get()); }
	if (layer == EventLayer::GUI)		{									  }
	if (layer == EventLayer::Engine)	{									  }

	// Handle the mouse scroll event.
	uint64_t layer = eventLog.mouseScroll->getLayer();
	if (layer == EventLayer::App)		{ onEvent(*eventLog.mouseScroll.get());	}
	if (layer == EventLayer::GUI)		{										}
	if (layer == EventLayer::Engine)	{										}

	// Events have been handled and the log can be cleared.
	eventLog.clear();
}

//==============================================================================================================================================//
//  Logging.																																	//
//==============================================================================================================================================//

void Application::logEvent(Event& event) 
{
	m_eventLog->log(event);
}

//==============================================================================================================================================//
//  On Events.																																	//
//==============================================================================================================================================//

void Application::onEvent(Event& event)
{
	// Get the ID.
	uint64_t eventID = event.getID();
	
	// Window events.																	 
	if		(eventID == EventType::WindowResize)	{ onWindowResizeEvent(dynamic_cast<WindowResizeEvent&>(event)); }
												 								 
	// Serialisation events.					 								 
	else if (eventID == EventType::FileDrop)		{ onFileDropEvent(dynamic_cast<FileDropEvent&>(event));			}
}

void Application::onWindowResizeEvent(WindowResizeEvent& event)
{
	// Check if the event has been handled.
	if (event.isHandled()) return;

	// Handle the event.


	// Done.
	event.handled();
}

void Application::onFileDropEvent(FileDropEvent& event)
{
	// Check if the event has been handled.
	if (event.isHandled()) return;

	// Handle the event.


	// Done.
	event.handled();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
