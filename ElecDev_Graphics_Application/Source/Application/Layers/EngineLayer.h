#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Layer.h"
#include <string>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EngineCore;
class GuiElementCore;
class GraphicsScene;

//==============================================================================================================================================//
//  Engine Layer.																																//
//==============================================================================================================================================//

class EngineLayer : public Layer
{
public:

	// Create an engine layer based on the ID.
	EngineLayer(LayerType engineType, std::string layerName, int imguiWindowFLags = 0);

	// Get the gui element in the layer.
	GuiElementCore* getGuiElement();

	// Get the engine in the layer.
	EngineCore* getEngine();

	// Pass an event to the layer.
	virtual void onEvent(Event& event) override;

	// Render the specific layer.
	virtual void onRender() override;

private:

	// The layer components.
	std::unique_ptr<EngineCore> m_engine = nullptr;
	std::unique_ptr<GraphicsScene> m_graphicsWindow = nullptr;

	// The imguiWindow flags related to the layer's ImGUI windows.
	int m_imguiWindowflags = 0;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//#pragma once
