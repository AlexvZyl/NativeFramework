#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <string>
#include "Lumen.h"
#include "Application/Application.h"
#include "Application/Layers/Layer.h"
#include "Application/Events/Events.h"
#include "Application/Layers/LayerStack.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Misc/ConsoleColor.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class GuiType>
class GuiLayer : public Layer
{
public:

	// Constructor.
	GuiLayer(std::string name, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GuiElementCore* getGuiElement();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the layer.
	virtual void onRender() override;

	// Dispatch the ImGUI events, if there are elements
	// in the layer that has to do this.
	virtual void dispatchEvents() override;

	// Set the name of all the elements in the layer.
	virtual void setName(std::string& newName) override;

	// Get the name of the layer.
	virtual std::string& getName() override;

	// Checks if the layer is hovered.
	virtual bool isHovered() override;

protected:

	// The GUI element that belongs to this layer.
	// The basic layer only has on Gui element.
	std::unique_ptr<GuiType> m_guiElement = nullptr;

};

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class GuiType>
GuiLayer<GuiType>::GuiLayer(std::string name, int imguiWindowFlags)
	: Layer()
{
	// Create GUI element.
	m_guiElement = std::make_unique<GuiType>(name, imguiWindowFlags);
}

template<class GuiType>
GuiElementCore* GuiLayer<GuiType>::getGuiElement()
{
	return m_guiElement.get();
}

template<class GuiType>
void GuiLayer<GuiType>::setName(std::string& newName)
{
	m_guiElement->m_name = newName;
}

template<class GuiType>
std::string& GuiLayer<GuiType>::getName()
{
	return m_guiElement->m_name;
}

template<class GuiType>
bool GuiLayer<GuiType>::isHovered() 
{
	return m_guiElement->isHovered();
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

template<class GuiType>
void GuiLayer<GuiType>::onEvent(Event& event)
{
	// The layer is responsible for passing the events coordinates as local to the window.

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

	if (m_guiElement->shouldRender())
		m_guiElement->onRender();

	m_guiElement->end();
	
	// Remove layer in next frame if close was clicked.
	if (!m_guiElement->m_isOpen)
		Lumen::getApp().queuePopLayer(this);
}

template<class GuiType>
void GuiLayer<GuiType>::dispatchEvents()
{
	// Add Gui events onto the layer events.
	m_guiElement->dispatchEvents();
}
//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//