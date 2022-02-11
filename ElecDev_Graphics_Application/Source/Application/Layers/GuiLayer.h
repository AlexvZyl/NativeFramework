#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <string>
#include "Lumen.h"
#include "Application/Layers/Layer.h"
#include "Application/Events/Events.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Application/Layers/LayerStack.h"
#include "Misc/ConsoleColor.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class GuiType>
class GuiLayer : public Layer
{
public:

	// Constructor.
	GuiLayer(std::string layerName, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GuiElementCore* getGuiElement();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the specific layer.
	virtual void onRender() override;

	// Dispatch events related to the layer.
	virtual void dispatchEvents() override;

	// Set the name of the layer elements.
	virtual void setName(std::string& newName) override;

protected:

	// The GUI element that belongs to this layer.
	// The basic layer only has on Gui element.
	std::unique_ptr<GuiType> m_guiElement = nullptr;

	// The imguiWindow flags related to the layer's ImGUI windows.
	int m_imguiWindowflags = 0;

};

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class GuiType>
GuiLayer<GuiType>::GuiLayer(std::string layerName, int imguiWindowFlags)
	: Layer(layerName), m_imguiWindowflags(imguiWindowFlags)
{
	// Create GUI element.
	m_guiElement = std::make_unique<GuiType>(layerName, imguiWindowFlags);
}

template<class GuiType>
GuiElementCore* GuiLayer<GuiType>::getGuiElement()
{
	return m_guiElement.get();
}

template<class GuiType>
void GuiLayer<GuiType>::setName(std::string& newName)
{
	m_layerName = newName;
	m_guiElement->m_name = newName;
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

template<class GuiType>
void GuiLayer<GuiType>::onEvent(Event& event)
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
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_guiElement->onEvent(mouseEvent);
	}
	else if (eventID == EventType_MouseScroll)
	{
		MouseScrollEvent mouseEvent = dynamic_cast<MouseScrollEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_guiElement->onEvent(mouseEvent);
	}
	else if (eventID == EventType_MousePress || eventID == EventType_MouseRelease)
	{
		MouseButtonEvent mouseEvent = dynamic_cast<MouseButtonEvent&>(event);
		mouseEvent.mousePosition.x = mouseEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		mouseEvent.mousePosition.y = mouseEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_guiElement->onEvent(mouseEvent);
	}

	// Key events.
	else if (eventID == EventType_KeyPress || eventID == EventType_KeyRelease || eventID == EventType_KeyRepeat)
	{
		KeyEvent keyEvent = dynamic_cast<KeyEvent&>(event);
		keyEvent.mousePosition.x = keyEvent.mousePosition.x - m_guiElement->m_contentRegionPosition.x;
		keyEvent.mousePosition.y = keyEvent.mousePosition.y - m_guiElement->m_contentRegionPosition.y;
		m_guiElement->onEvent(keyEvent);
	}

	// The other events do not need adjustments.
	else m_guiElement->onEvent(event);
}

template<class GuiType>
void GuiLayer<GuiType>::onRender()
{
	m_guiElement->begin();
	m_guiElement->onRender();
	m_guiElement->end();

	// Remove layer in next frame if close was clicked.
	if (!m_guiElement->m_isOpen)
		Lumen::getApp().queuePopLayer(this);
}

template<class GuiType>
void GuiLayer<GuiType>::dispatchEvents()
{
	// Ensure window exists.
	if (!m_imGuiWindow) return;

	// Call layer dispatcher.
	Layer::dispatchEvents();

	// Add Gui events onto the layer events.
	m_guiElement->dispatchGuiEvents(m_imGuiWindow);
}
//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//