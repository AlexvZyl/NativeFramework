//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "BasicGuiLayer.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "GUI/Ribbon/Ribbon.h"
#include "GUI/Toolbar/toolbar.h"
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
		m_guiElement = std::make_unique<ComponentEditor>(m_layerName, m_imguiWindowflags);
	}

	else if (ID == LayerType_Toolbar)
	{
		m_guiElement = std::make_unique<Toolbar>(m_layerName, m_imguiWindowflags);
	}
	else if (ID == LayerType_Ribbon)
	{
		m_guiElement = std::make_unique<Ribbon>(m_layerName, m_imguiWindowflags);
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

void BasicGuiLayer::changeName(std::string& name) 
{
	m_layerName = name;
	m_guiElement->m_name = name;
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void BasicGuiLayer::onEvent(Event& event)
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

void BasicGuiLayer::onRender()
{
	m_guiElement->begin();
	m_guiElement->renderBody();
	m_guiElement->end();
}

void BasicGuiLayer::dispatchLayerEvents() 
{
	// Ensure window exists.
	if (!m_imGuiWindow) return;

	// Call layer dispatcher.
	Layer::dispatchLayerEvents();

	// Add Gui events onto the layer events.
	m_guiElement->dispatchGuiEvents(m_imGuiWindow);
}
//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//