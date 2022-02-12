#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Application/Layers/GuiLayer.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class EngineType>
class EngineLayer : public GuiLayer<GraphicsScene>
{
public:

	// Constructor.
	EngineLayer(std::string name, int imguiWindowFLags = 0)
		: GuiLayer<GraphicsScene>(name, imguiWindowFLags)
	{
		// Create the engine.
		m_engine = std::make_unique<EngineType>();
		// Set the texture.
		m_guiElement->setEngine(m_engine.get());
	}

	// Get the engine in the layer.
	inline EngineType* getEngine() 
	{
		return m_engine.get();
	}

	// Pass an event to the layer.
	virtual void onEvent(Event& event);

	// Render the layer.
	virtual void onRender();

protected:

	// The engine that belongs to the layer.
	std::unique_ptr<EngineType> m_engine = nullptr;

};

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

template<class EngineType>
void EngineLayer<EngineType>::onEvent(Event& event)
{
	// Global events are created using GLFW coordinates.  They
	// have to be converted to coordinates that are local to the 
	// window's content region.
	// The event is copied (instead of using a reference) so that it can 
	// still be used in other places throughout Lumen.

	// Since we now have access to the windows outside of the loop we can 
	// look into using imgui as the event dispatcher instead of using
	// glfw directly.  This prevents us from having to create multiple instaces
	// of events.  And what if an event is already passed as local coordinates?

	// Log the event.
	std::cout << m_guiElement->m_name << ": " << event.ID << "\n";

	uint64_t eventID = event.ID;

	// Mouse events.
	if (eventID == EventType_MouseMove)
	{
		MouseMoveEvent mouseEvent = dynamic_cast<MouseMoveEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_engine->onEvent(mouseEvent);
		m_guiElement->onEvent(mouseEvent);
	}
	else if (eventID == EventType_MouseScroll)
	{
		MouseScrollEvent mouseEvent = dynamic_cast<MouseScrollEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_engine->onEvent(mouseEvent);
		m_guiElement->onEvent(mouseEvent);
	}
	else if (eventID == EventType_MousePress || eventID == EventType_MouseRelease)
	{
		MouseButtonEvent mouseEvent = dynamic_cast<MouseButtonEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_engine->onEvent(mouseEvent);
		m_guiElement->onEvent(mouseEvent);
	}

	// Key events.
	else if (eventID == EventType_KeyPress || eventID == EventType_KeyRelease || eventID == EventType_KeyRepeat)
	{
		KeyEvent keyEvent = dynamic_cast<KeyEvent&>(event);
		keyEvent.mousePosition.x = keyEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		keyEvent.mousePosition.y = keyEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_engine->onEvent(keyEvent);
		m_guiElement->onEvent(keyEvent);
	}

	// The other events do not need adjustments.
	else
	{
		m_engine->onEvent(event);
		m_guiElement->onEvent(event);
	}
}

template<class EngineType>
void EngineLayer<EngineType>::onRender()
{
	m_guiElement->begin();

	// Check if should render.
	if (m_guiElement->shouldRender()) 
	{
		// Render.
		m_engine->onRender();
		m_guiElement->onRender();
		m_guiElement->end();
	}

	// Remove layer in next frame if close was clicked.
	if (!m_guiElement->m_isOpen)
		Lumen::getApp().queuePopLayer(this);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//