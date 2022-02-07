//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "BasicGuiLayer.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Misc/ConsoleColor.h"
#include "Application/Events/Events.h"
#include "Engines/EngineCore/EngineCore.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

BasicGuiLayer::BasicGuiLayer(uint64_t ID, std::string layerName, int imguiWindowFlags)
	: Layer(ID | LayerType_GUI, layerName), m_imguiWindowflags(imguiWindowFlags)
{
	// --------------- //
	//  E N G I N E S  //
	// --------------- //

	if (ID == LayerType_Base2DEngine   ||
		ID == LayerType_Base3DEngine   ||
		ID == LayerType_Design2DEngine ||
		ID == LayerType_Design3DEngine)
	{
		m_layerID |= LayerType_GraphicsScene | LayerType_GUI | LayerType_Engine;
		m_guiElement = std::make_unique<GraphicsScene>(ID, m_layerName, m_imguiWindowflags);
	}

	// ------------------------- //
	//  G U I   E L E M E N T S  //
	// ------------------------- //

	else if (ID == LayerType_ComponentEditor)
	{
		m_layerID |= LayerType_GUI;
		m_guiElement = std::make_unique<ComponentEditor>(m_layerName, m_imguiWindowflags);
	}

	// ----------- //
	//  E R R O R  //
	// ----------- //

	else
	{
		std::cout << red << "\n[LAYERS] [ERROR]: " << white << "Could not create GuiLayer based on the provided ID.\n";
		assert(true);
	}
}

GuiElementCore* BasicGuiLayer::getGuiElement()
{
	return m_guiElement.get();
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void BasicGuiLayer::onEvent(Event& event)
{
	// Events that contain mouse coordinates have to be converted to
	// local window coordinates.

	uint64_t eventID = event.getID();

	// Mouse events.
	if (eventID == EventType_MouseMove)
	{
		// Construct a new event to pass to the engine.
		MouseMoveEvent& layerEvent = dynamic_cast<MouseMoveEvent&>(event);
		glm::vec2 localCoords = layerEvent.getPosition();
		localCoords.x = localCoords.x - m_guiElement->m_contentRegionPosition.x - m_layerPosition.x;
		localCoords.y = localCoords.y - m_guiElement->m_contentRegionPosition.y - m_layerPosition.y;
		MouseMoveEvent localEvent(localCoords, eventID);
		m_guiElement->onEvent(localEvent);
	}
	else if (eventID == EventType_MouseScroll)
	{
		// Construct a new event to pass to the engine.
		MouseScrollEvent& layerEvent = dynamic_cast<MouseScrollEvent&>(event);
		glm::vec2 localCoords = layerEvent.getPosition();
		localCoords.x = localCoords.x - m_guiElement->m_contentRegionPosition.x - m_layerPosition.x;
		localCoords.y = localCoords.y - m_guiElement->m_contentRegionPosition.y - m_layerPosition.y;
		MouseScrollEvent localEvent(localCoords, layerEvent.getYOffset(), eventID);
		m_guiElement->onEvent(localEvent);
	}
	else if (eventID == EventType_MousePress || eventID == EventType_MouseRelease)
	{
		// Construct a new event to pass to the engine.
		MouseButtonEvent& layerEvent = dynamic_cast<MouseButtonEvent&>(event);
		glm::vec2 localCoords = layerEvent.getPosition();
		localCoords.x = localCoords.x - m_guiElement->m_contentRegionPosition.x - m_layerPosition.x;
		localCoords.y = localCoords.y - m_guiElement->m_contentRegionPosition.y - m_layerPosition.y;
		MouseButtonEvent localEvent(localCoords, eventID);
		m_guiElement->onEvent(localEvent);
	}

	// Key events.
	else if (eventID == EventType_KeyPress || eventID == EventType_KeyRelease || eventID == EventType_KeyRepeat)
	{
		// Construct a new event to pass to the engine.
		KeyEvent& layerEvent = dynamic_cast<KeyEvent&>(event);
		glm::vec2 localCoords = layerEvent.getMousePosition();
		localCoords.x = localCoords.x - m_guiElement->m_contentRegionPosition.x - m_layerPosition.x;
		localCoords.y = localCoords.y - m_guiElement->m_contentRegionPosition.y - m_layerPosition.y;
		KeyEvent localEvent(layerEvent.getKey(), eventID, localCoords);
		m_guiElement->onEvent(localEvent);
	}

	// The other events do not need adjustments.
	else
	{
		m_guiElement->onEvent(event);
	}

	std::cout << m_guiElement->m_name << ": " << event.getID() << "\n";
}

void BasicGuiLayer::onRender()
{
	m_guiElement->begin();
	updateLayerData();
	m_guiElement->renderBody();
	m_guiElement->end();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//