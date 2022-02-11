#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <string>
#include "Lumen.h"
#include "Application/Layers/Layer.h"
#include "Application/Events/Events.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Application/Layers/LayerStack.h"
#include "Misc/ConsoleColor.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class EngineType>
class EngineLayer : public Layer
{
public:

	// Constructor.
	EngineLayer(std::string layerName, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GraphicsScene* getGuiElement();

	// Get a pointer to the engine.
	EngineType* getEngine();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the specific layer.
	virtual void onRender() override;

	// Dispatch events related to the layer.
	virtual void dispatchEvents() override;

	// Set the names of the layer elements.
	virtual void setName(std::string& newName) override;

protected:

	// The GUI element that belongs to this layer.
	std::unique_ptr<GraphicsScene> m_graphicsScene = nullptr;
	// The engine that belongs to the layer.
	std::unique_ptr<EngineType> m_engine = nullptr;

	// The imguiWindow flags related to the layer's ImGUI windows.
	int m_imguiWindowflags = 0;

};

//==============================================================================================================================================//
//  Engine Layer.																																	//
//==============================================================================================================================================//

template<class EngineType>
EngineLayer<EngineType>::EngineLayer(std::string layerName, int imguiWindowFlags)
	: Layer(layerName), m_imguiWindowflags(imguiWindowFlags)
{
	// Create GUI element.
	m_graphicsScene = std::make_unique<GraphicsScene>(layerName, imguiWindowFlags);
	// Create the engine.
	m_engine = std::make_unique<EngineType>();
	// Set the texture.
	m_graphicsScene->setEngine(m_engine.get());
}

template<class EngineType>
GraphicsScene* EngineLayer<EngineType>::getGuiElement()
{
	return m_graphicsScene.get();
}

template<class EngineType>
EngineType* EngineLayer<EngineType>::getEngine()
{
	return m_engine.get();
}

template<class EngineType>
void EngineLayer<EngineType>::setName(std::string& newName) 
{
	m_layerName = newName;
	m_graphicsScene->m_name = newName;
}

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
	std::cout << m_layerName << ": " << event.ID << "\n";

	uint64_t eventID = event.ID;

	// Mouse events.
	if (eventID == EventType_MouseMove)
	{
		MouseMoveEvent mouseEvent = dynamic_cast<MouseMoveEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_graphicsScene->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_graphicsScene->m_contentRegionPosition.y;
		m_engine->onEvent(mouseEvent);
	}
	else if (eventID == EventType_MouseScroll)
	{
		MouseScrollEvent mouseEvent = dynamic_cast<MouseScrollEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_graphicsScene->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_graphicsScene->m_contentRegionPosition.y;
		m_engine->onEvent(mouseEvent);
	}
	else if (eventID == EventType_MousePress || eventID == EventType_MouseRelease)
	{
		MouseButtonEvent mouseEvent = dynamic_cast<MouseButtonEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_graphicsScene->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_graphicsScene->m_contentRegionPosition.y;
		m_engine->onEvent(mouseEvent);
	}

	// Key events.
	else if (eventID == EventType_KeyPress || eventID == EventType_KeyRelease || eventID == EventType_KeyRepeat)
	{
		KeyEvent keyEvent = dynamic_cast<KeyEvent&>(event);
		keyEvent.mousePosition.x = keyEvent.mousePosition.x - m_graphicsScene->m_contentRegionPosition.x;
		keyEvent.mousePosition.y = keyEvent.mousePosition.y - m_graphicsScene->m_contentRegionPosition.y;
		m_engine->onEvent(keyEvent);
	}

	// The other events do not need adjustments.
	else m_engine->onEvent(event);
}

template<class EngineType>
void EngineLayer<EngineType>::onRender()
{
	m_graphicsScene->begin();

	// Check if should render.
	if (!m_graphicsScene->shouldRender()) return;

	// Render.
	m_engine->onRender();
	m_graphicsScene->onRender();
	m_graphicsScene->end();

	// Remove layer in next frame if close was clicked.
	if (!m_graphicsScene->m_isOpen)
		Lumen::getApp().queuePopLayer(this);
}

template<class EngineType>
void EngineLayer<EngineType>::dispatchEvents()
{
	// Ensure window exists.
	if (!m_imGuiWindow) return;

	// Call layer dispatcher.
	Layer::dispatchEvents();

	// Add Gui events onto the layer events.
	m_graphicsScene->dispatchGuiEvents(m_imGuiWindow);
}
//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//