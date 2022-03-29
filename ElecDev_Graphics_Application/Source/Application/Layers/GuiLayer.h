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
#include "External/Misc/ConsoleColor.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

template<class GuiType>
class GuiLayer : public Layer
{
public:

	// Constructor.
	GuiLayer(std::string name, int imguiWindowFLags = 0);

	// Destructor.
	inline virtual ~GuiLayer() = default;

	// Get the gui element in the layer.
	GuiType* getGui();

	// ----------- //
	//  L A Y E R  //	
	// ----------- //

	virtual void onEvent(Event& event) override;
	virtual void onRender() override;
	virtual void onUpdate() override;
	virtual void setName(std::string newName) override;
	virtual std::string getName() override;
	virtual bool isHovered() override;
	virtual void focus() override;

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
	m_guiElement->m_layer = this;
}

template<class GuiType>
GuiType* GuiLayer<GuiType>::getGui()
{
	return m_guiElement.get();
}

template<class GuiType>
void GuiLayer<GuiType>::setName(std::string newName)
{
	std::string name = newName + "###LumenLayer" + std::to_string(m_ID);
	m_guiElement->m_name = name;
}

template<class GuiType>
std::string GuiLayer<GuiType>::getName()
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
	// The layer is responsible for passing the events coordinates as local to the window,
	// since these are GLFW events that whose coordinates are in the entire window.

	if (event.isConsumed()) return;

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
	// Begin the window.
	m_guiElement->begin(); 

	// Render contents.
	if (m_guiElement->shouldRender())
		m_guiElement->onRender();

	// End window.
	m_guiElement->end();
	
	// Remove layer if close was clicked.
	if (!m_guiElement->m_isOpen)
		Lumen::getApp().queuePopLayer(this);
}

template<class GuiType>
void GuiLayer<GuiType>::onUpdate()
{
	m_guiElement->onUpdate();
}

template<class GuiType>
void GuiLayer<GuiType>::focus() 
{
	ImGui::SetWindowFocus(m_guiElement->m_name.c_str());
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//