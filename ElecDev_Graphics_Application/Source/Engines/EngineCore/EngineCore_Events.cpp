//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Engines/EngineCore/EngineCore.h"
#include "Application/Events/Events.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Lumen.h"
#include "OpenGL/Primitives/Grid.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  On Event.																																	//
//==============================================================================================================================================//

void EngineCore::onEvent(Event& event)
{
	if (event.isConsumed()) return;

	uint64_t eventID = event.ID;

	// Mouse events.
	if		(eventID == EventType_MousePress)	{ onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }
	else if (eventID == EventType_MouseRelease) { onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event)); }
	else if (eventID == EventType_MouseMove)	{ onMouseMoveEventForce(dynamic_cast<MouseMoveEvent&>(event)); }
	else if (eventID == EventType_MouseScroll)	{ onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(event)); }
	else if (eventID == EventType_MouseDrag)	{ onMouseDragEvent(dynamic_cast<MouseDragEvent&>(event)); }

	// Key events.
	else if (eventID == EventType_KeyPress)		{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }
	else if (eventID == EventType_KeyRelease)	{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }

	// Window events.
	else if (eventID == EventType_WindowResize) { onWindowResizeEventForce(dynamic_cast<WindowEvent&>(event)); }

	// Notify.
	else if (eventID == EventType_Notify)		{ onNotifyEventForce(dynamic_cast<NotifyEvent&>(event)); }

	// File events.
	else if (eventID == EventType_FileDrop)		{ onFileDropEvent(dynamic_cast<FileDropEvent&>(event)); }

	// Event not found.
	else LUMEN_LOG_WARN("Invalid event ID.", "Engine Events");
}

//==============================================================================================================================================//
//  Forced parent event calls.																													//
//==============================================================================================================================================//

void EngineCore::onWindowResizeEventForce(WindowEvent& event)
{
	m_scene->resize(event.windowData);
	m_contentRegionSize = event.windowData;

	onWindowResizeEvent(event);
}

void EngineCore::onFocusEventForce(NotifyEvent& event) 
{
	m_isFocused = true;
	Renderer::bindScene(m_scene.get());
	Lumen::getApp().setActiveEngine(this);

	onFocusEvent(event);
}

void EngineCore::onDefocusEventForce(NotifyEvent& event) 
{
	m_isFocused = false;
	// We not unbind the scene here, since focus can sometimes shift to 
	// another window, but that window is editing things in this engine.
	// By not unbinding here we always keep the latest focused engine's
	// scene active.

	onDefocusEvent(event);
}

void EngineCore::onHoverEventForce(NotifyEvent& event) 
{
	m_isHovered = true;

	onHoverEvent(event);
}

void EngineCore::onDehoverEventForce(NotifyEvent& event) 
{
	m_isHovered = false;

	onDehoverEvent(event);
}

void EngineCore::onNotifyEventForce(NotifyEvent& event)
{
	uint64_t eventID = event.ID;
	if		(eventID == EventType_Focus)	{ onFocusEventForce(event); }
	else if (eventID == EventType_Defocus)	{ onDefocusEventForce(event); }
	else if (eventID == EventType_Hover)	{ onHoverEventForce(event); }
	else if (eventID == EventType_Dehover)	{ onDehoverEventForce(event); }

	onNotifyEvent(event);
}

void EngineCore::onMouseMoveEventForce(MouseMoveEvent& event)
{
	m_scene->m_grid->updateHelperCircle(m_scene->pixelCoordsToWorldCoords(getMouseLocalPosition()));

	onMouseMoveEvent(event);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//