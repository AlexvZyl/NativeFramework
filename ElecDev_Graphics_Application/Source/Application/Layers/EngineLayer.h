#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Application/Layers/GuiLayer.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

// The engine layer does not have to pass events on to the engine,
// since GraphicsScene does that.  Need to think about if this is the
// best way to do it.

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

	// Destructor.
	inline virtual ~EngineLayer() = default;

	// Get the engine in the layer.
	inline EngineType* getEngine() 
	{
		return m_engine.get();
	}

protected:

	// The engine that belongs to the layer.
	std::unique_ptr<EngineType> m_engine = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//