#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Layer.h"
#include <string>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class GuiElementCore;
class Event;

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

class BasicGuiLayer : public Layer
{
public:

	// Create a GUI layer based on the ID.
	BasicGuiLayer(uint64_t ID, std::string layerName, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GuiElementCore* getGuiElement();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the specific layer.
	virtual void onRender() override;

protected:

	// The GUI element that belongs to this layer.
	// The basic layer only has on Gui element.
	std::unique_ptr<GuiElementCore> m_guiElement = nullptr;

	// The imguiWindow flags related to the layer's ImGUI windows.
	int m_imguiWindowflags = 0;

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//