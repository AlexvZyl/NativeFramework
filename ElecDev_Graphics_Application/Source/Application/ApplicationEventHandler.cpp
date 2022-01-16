//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application.h"
#include "Events/Events.h"
#include "Graphics/graphicsHandler.h"

//==============================================================================================================================================//
//  General.																																	//
//==============================================================================================================================================//

void Application::handleEvents()
{
	// Key events.
	for (std::unique_ptr<KeyEvent>& event : eventLog.keyPressEvents)	{ handleKeyPressEvent(event);	}
	for (std::unique_ptr<KeyEvent>& event : eventLog.keyReleaseEvents)	{ handleKeyReleaseEvent(event);	}

	// Mouse events.
	if (eventLog.mouseMoveEvent)			{ handleMouseMoveEvent(eventLog.mouseMoveEvent);		}
	if (eventLog.mouseScrollEvent)			{ handleMouseScrollEvent(eventLog.mouseScrollEvent);	}
	if (eventLog.mousePressEvent)			{ handleMousePressEvent(eventLog.mousePressEvent);		}
	if (eventLog.mouseReleaseEvent)			{ handleMouseReleaseEvent(eventLog.mouseReleaseEvent);	}

	// Window events.
	if (eventLog.windowResizeEvent)			{ handleWindowResizeEvent(eventLog.windowResizeEvent);	}

	// Serialisation events.
	if (eventLog.fileDropEvent)				{ handleFileDropEvent(eventLog.fileDropEvent);			}

	// All events have been handled.
	eventLog.clear();
}

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

void Application::handleMouseMoveEvent(std::unique_ptr<MouseMoveEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;
	
	// Handle the event.


}

void Application::handleMouseScrollEvent(std::unique_ptr<MouseScrollEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.

}

void Application::handleMousePressEvent(std::unique_ptr<MouseButtonEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.


}

void Application::handleMouseReleaseEvent(std::unique_ptr<MouseButtonEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.


}

//==============================================================================================================================================//
//  Key events.																																	//
//==============================================================================================================================================//

void Application::handleKeyPressEvent(std::unique_ptr<KeyEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.

}

void Application::handleKeyReleaseEvent(std::unique_ptr<KeyEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.


}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

void Application::handleWindowResizeEvent(std::unique_ptr<WindowResizeEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.


}

//==============================================================================================================================================//
//  Serialisation events.																														//
//==============================================================================================================================================//

void Application::handleFileDropEvent(std::unique_ptr<FileDropEvent>& event)
{
	// Check if the event has been handled.
	if (event->isHandled()) return;

	// Handle the event.


}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
