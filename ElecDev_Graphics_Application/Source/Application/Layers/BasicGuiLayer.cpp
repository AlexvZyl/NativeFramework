//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "BasicGuiLayer.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "Misc/ConsoleColor.h"
#include "Application/Events/Events.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

BasicGuiLayer::BasicGuiLayer(LayerType guiElement, std::string layerName, int imguiWindowFlags)
	: Layer(guiElement | LayerType_GUI, layerName), m_imguiWindowflags(imguiWindowFlags)
{
	// Create the Basic GUI element.
	if (guiElement == LayerType_ComponentEditor) m_guiElement = std::make_unique<ComponentEditor>(m_layerName, m_imguiWindowflags);

	// Log error.
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

void BasicGuiLayer::onEvent(Event& event)
{
	m_guiElement->onEvent(event);
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