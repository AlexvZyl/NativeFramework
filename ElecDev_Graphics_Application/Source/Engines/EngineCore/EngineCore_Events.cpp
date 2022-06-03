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
#include "GUI/LumenGizmo/LumenGizmo.h"
#include "Application/LumenWindow/LumenWindow.h"

//==============================================================================================================================================//
//  On Event.																																	//
//==============================================================================================================================================//

void EngineCore::onEvent(const Event& event)
{
	if (event.isConsumed()) return;

	// Mouse events.
	if		(event.isType(EventType_MouseMove))			{ onMouseMoveEventForce(event.cast<MouseMoveEvent>()); }
	else if (event.isType(EventType_MouseDrag))			{ onMouseDragEventForce(event.cast<MouseDragEvent>()); }
	else if (event.isType(EventType_MouseScroll))		{ onMouseScrollEvent(event.cast<MouseScrollEvent>()); }
	else if	(event.isType(EventType_MousePress))		{ onMouseButtonEventForce(event.cast<MouseButtonEvent>()); }
	else if (event.isType(EventType_MouseRelease))		{ onMouseButtonEventForce(event.cast<MouseButtonEvent>()); }
	else if (event.isType(EventType_MouseDoublePress))	{ onMouseButtonEventForce(event.cast<MouseButtonEvent>()); }

	// Key events.
	else if (event.isType(EventType_KeyPress)
		  || event.isType(EventType_KeyRelease)
		  || event.isType(EventType_KeyRepeat))			{ onKeyEvent(event.cast<KeyEvent>()); }

	// Window events.
	else if (event.isType(EventType_WindowResize))		{ onWindowResizeEventForce(event.cast<WindowEvent>()); }
	else if (event.isType(EventType_WindowMove))		{}

	// Notify.
	else if (event.isType(EventType_Notify))			{ onNotifyEventForce(event.cast<NotifyEvent>()); }

	// File events.
	else if (event.isType(EventType_FileDrop))			{ onFileDropEventForce(event.cast<FileDropEvent>()); }
	else if (event.isType(EventType_YamlNodeDrop))		{ onYamlNodeDropEventForce(event.cast<YamlNodeDropEvent>()); }

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
	getGizmo()->enable();
	onFocusEvent(event);
}

void EngineCore::onDefocusEventForce(const NotifyEvent& event) 
{
	m_isFocused = false;
	getGizmo()->disable();
	// We not unbind the scene here, since focus can sometimes shift to 
	// another window, but that window is editing things in this engine.
	// By not unbinding here we always keep the latest focused engine's
	// scene active.
	onDefocusEvent(event);
}

void EngineCore::onHoverEventForce(const NotifyEvent& event) 
{
	m_isHovered = true;
	if (getScene().getGrid().m_helperCircleEnabled)
		m_scene->m_grid->visibleHelperCircle();
	onHoverEvent(event);
}

void EngineCore::onDehoverEventForce(const NotifyEvent& event) 
{
	m_isHovered = false;
	getScene().getGrid().hideHelperCircle();
	onDehoverEvent(event);
}

void EngineCore::onMouseDragEventForce(const MouseDragEvent& event) 
{
	if (   event.isType(EventType_MouseButtonMiddle)
		|| event.isType(EventType_MouseButtonLeft | EventType_LeftCtrl)
		|| !getGizmo()->isOver())
	{
		onMouseDragEvent(event);
	}
}

void EngineCore::onMouseButtonEventForce(const MouseButtonEvent& event) 
{
	if (!m_isHovered ||getGizmo()->isOver()) return;
	onMouseButtonEvent(event);
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
	if(getScene().getGrid().m_helperCircleEnabled)
		getScene().getGrid().updateHelperCircle(pixelToWorldCoords(getMouseLocalPosition()));
	onMouseMoveEvent(event);
}

void EngineCore::onFileDropEventForce(const FileDropEvent& event) 
{
	m_parentWindow->focus();
	onFileDropEvent(event);
}

void EngineCore::onYamlNodeDropEventForce(const YamlNodeDropEvent& event) 
{
	m_parentWindow->focus();
	onYamlNodeDropEvent(event);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//