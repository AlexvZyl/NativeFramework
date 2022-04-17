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

void EngineCore::onEvent(const Event& event)
{
	if (event.isConsumed()) return;

	// Mouse events.
	if		(event.isType(EventType_MouseMove))			{ onMouseMoveEventForce(dynamic_cast<const MouseMoveEvent&>(event)); }
	else if (event.isType(EventType_MouseDrag))			{ onMouseDragEvent(dynamic_cast<const MouseDragEvent&>(event)); }
	else if	(event.isType(EventType_MousePress))		{ onMouseButtonEvent(dynamic_cast<const MouseButtonEvent&>(event)); }
	else if (event.isType(EventType_MouseRelease))		{ onMouseButtonEvent(dynamic_cast<const MouseButtonEvent&>(event)); }
	else if (event.isType(EventType_MouseDoublePress))	{ onMouseButtonEvent(dynamic_cast<const MouseButtonEvent&>(event)); }
	else if (event.isType(EventType_MouseScroll))		{ onMouseScrollEvent(dynamic_cast<const MouseScrollEvent&>(event)); }

	// Key events.
	else if (event.isType(EventType_KeyPress))			{ onKeyEvent(dynamic_cast<const KeyEvent&>(event)); }
	else if (event.isType(EventType_KeyRelease))		{ onKeyEvent(dynamic_cast<const KeyEvent&>(event)); }

	// Window events.
	else if (event.isType(EventType_WindowResize))		{ onWindowResizeEventForce(dynamic_cast<const WindowEvent&>(event)); }
	else if (event.isType(EventType_WindowMove))		{}

	// Notify.
	else if (event.isType(EventType_Notify))			{ onNotifyEventForce(dynamic_cast<const NotifyEvent&>(event)); }

	// File events.
	else if (event.isType(EventType_FileDrop))			{ onFileDropEvent(dynamic_cast<const FileDropEvent&>(event)); }

	// Event unhandled.
	else LUMEN_LOG_WARN("No handler for event.", "Engine Core");
}

//==============================================================================================================================================//
//  Forced parent event calls.																													//
//==============================================================================================================================================//

void EngineCore::onWindowResizeEventForce(const WindowEvent& event)
{
	m_scene->resize(event.windowData);
	onWindowResizeEvent(event);
}

void EngineCore::onFocusEventForce(const NotifyEvent& event) 
{
	m_isFocused = true;
	Renderer::bindScene(m_scene.get());
	Lumen::getApp().setActiveEngine(this);
	onFocusEvent(event);
}

void EngineCore::onDefocusEventForce(const NotifyEvent& event) 
{
	m_isFocused = false;
	// We not unbind the scene here, since focus can sometimes shift to 
	// another window, but that window is editing things in this engine.
	// By not unbinding here we always keep the latest focused engine's
	// scene active.
	onDefocusEvent(event);
}

void EngineCore::onHoverEventForce(const NotifyEvent& event) 
{
	m_isHovered = true;
	if(m_scene->m_grid->m_helperCircleEnabled)
		m_scene->m_grid->visibleHelperCircle();
	onHoverEvent(event);
}

void EngineCore::onDehoverEventForce(const NotifyEvent& event) 
{
	m_isHovered = false;
	m_scene->m_grid->hideHelperCircle();
	onDehoverEvent(event);
}

void EngineCore::onNotifyEventForce(const NotifyEvent& event)
{
	if		(event.isType(EventType_Focus))		{ onFocusEventForce(event);	  }
	else if (event.isType(EventType_Defocus))	{ onDefocusEventForce(event); }
	else if (event.isType(EventType_Hover))		{ onHoverEventForce(event);	  }
	else if (event.isType(EventType_Dehover))	{ onDehoverEventForce(event); }
	else										{ onNotifyEvent(event);		  }
}

void EngineCore::onMouseMoveEventForce(const MouseMoveEvent& event)
{
	m_scene->m_grid->updateHelperCircle(m_scene->pixelCoordsToWorldCoords(getMouseLocalPosition()));
	onMouseMoveEvent(event);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//