//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Engines/EngineCore/EngineCore.h"
#include "Application/Events/Events.h"
#include "OpenGL/RendererGL.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  On Event.																																	//
//==============================================================================================================================================//

void EngineCore::onEvent(Event& event)
{
	uint64_t eventID = event.ID;

	// Mouse events.
	if		(eventID == EventType_MouseMove)	{ onMouseMoveEvent(dynamic_cast<MouseMoveEvent&>(event));		}
	else if (eventID == EventType_MouseScroll)	{ onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(event));	}
	else if (eventID == EventType_MousePress)	{ onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event));	}
	else if (eventID == EventType_MouseRelease) { onMouseButtonEvent(dynamic_cast<MouseButtonEvent&>(event));	}

	// Key events.
	else if (eventID == EventType_KeyPress)		{ onKeyEvent(dynamic_cast<KeyEvent&>(event));					}
	else if (eventID == EventType_KeyRelease)	{ onKeyEvent(dynamic_cast<KeyEvent&>(event));					}

	// Window events.
	else if (eventID == EventType_WindowResize) { onWindowResizeEvent(dynamic_cast<WindowEvent&>(event)); }

	// Layer events.
	else if (eventID == EventType_Focus)		{ onFocusEvent(dynamic_cast<LayerEvent&>(event));				}
	else if (eventID == EventType_Defocus)		{ onDefocusEvent(dynamic_cast<LayerEvent&>(event));				}
	else if (eventID == EventType_Hover)		{ onHoverEvent(dynamic_cast<LayerEvent&>(event));				}
	else if (eventID == EventType_Dehover)		{ onDehoverEvent(dynamic_cast<LayerEvent&>(event));				}

	// Pass the event on to the scene, since the scene contains the camera.
	m_scene->onEvent(event);
}

//==============================================================================================================================================//
//  Window Events.																																//
//==============================================================================================================================================//

void EngineCore::onWindowResizeEvent(WindowEvent& event)
{
	glm::vec2 eventSize = event.windowData;
	m_scene->resize((int)eventSize.x, (int)eventSize.y);
}

//==============================================================================================================================================//
//  Layer Events.																																//
//==============================================================================================================================================//

void EngineCore::onFocusEvent(LayerEvent& event)
{
	m_isFocused = true;
	Renderer::bindScene(m_scene.get());
}

void EngineCore::onDefocusEvent(LayerEvent& event)
{
	m_isFocused = false;
	Renderer::unbindScene();
}

void EngineCore::onHoverEvent(LayerEvent& event)
{
	m_isHovered = true;
}

void EngineCore::onDehoverEvent(LayerEvent& event)
{
	m_isHovered = false;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//