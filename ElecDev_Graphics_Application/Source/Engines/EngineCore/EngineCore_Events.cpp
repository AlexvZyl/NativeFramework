//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Engines/EngineCore/EngineCore.h"
#include "Application/Events/Events.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Lumen.h"

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
	else if (eventID == EventType_MouseMove)	{ onMouseMoveEvent(dynamic_cast<MouseMoveEvent&>(event)); }
	else if (eventID == EventType_MouseScroll)	{ onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(event)); }

	// Key events.
	else if (eventID == EventType_KeyPress)		{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }
	else if (eventID == EventType_KeyRelease)	{ onKeyEvent(dynamic_cast<KeyEvent&>(event)); }

	// Window events.
	else if (eventID == EventType_WindowResize) { onWindowResizeEvent(dynamic_cast<WindowEvent&>(event)); }

	// Layer events.
	else if (eventID == EventType_Focus)		{ onFocusEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Defocus)		{ onDefocusEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Hover)		{ onHoverEvent(dynamic_cast<LayerEvent&>(event)); }
	else if (eventID == EventType_Dehover)		{ onDehoverEvent(dynamic_cast<LayerEvent&>(event)); }

	// File events.
	else if (eventID == EventType_FileDrop)		{ onFileDropEvent(dynamic_cast<FileDropEvent&>(event)); }

	// Pass event to scene (sometimes the scene has to respond to events).
	m_scene->onEvent(event);
}

//==============================================================================================================================================//
//  Window Events.																																//
//==============================================================================================================================================//

void EngineCore::onWindowResizeEvent(WindowEvent& event)
{
	m_scene->resize(event.windowData);
	m_contentRegionSize = event.windowData;
}

//==============================================================================================================================================//
//  Layer Events.																																//
//==============================================================================================================================================//

void EngineCore::onFocusEvent(LayerEvent& event)
{
	m_isFocused = true;
	Renderer::bindScene(m_scene.get());
	Lumen::getApp().setActiveEngine(this);
}

void EngineCore::onDefocusEvent(LayerEvent& event)
{
	m_isFocused = false;
	// We not unbind the scene here, since focus can sometimes shift to 
	// another window, but that window is editing things in this engine.
	// By not unbinding here we always keep the latest focused engine's
	// scene active.
}

void EngineCore::onHoverEvent(LayerEvent& event)
{
	m_isHovered = true;
	//m_prevMouseEventPixelCoords = getMousePosition();
}

void EngineCore::onDehoverEvent(LayerEvent& event)
{
	m_isHovered = false;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//