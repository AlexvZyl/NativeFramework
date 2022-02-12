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
	virtual void onEvent(Event& event) override;

	// Render the layer.
	virtual void onRender() override;

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
	// The layer is responsible for passing the events coordinates as local to the window.

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
	}
	m_guiElement->end();

	// Remove layer in next frame if close was clicked.
	if (!m_guiElement->m_isOpen)
		Lumen::getApp().queuePopLayer(this);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//