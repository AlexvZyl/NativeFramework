#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Application/Layers/GuiLayer.h"

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

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

	// Render the layer.
	virtual void onRender() override 
	{
		// Begin the window.
		m_guiElement->begin();

		// Render contents.
		if (m_guiElement->shouldRender())
		{
			m_engine->onRender();
			m_guiElement->onRender();
		}

		// End window.
		m_guiElement->end();

		// Remove layer in next frame if close was clicked.
		if (!m_guiElement->m_isOpen)
			Lumen::getApp().queuePopLayer(this);
	}

	// The engine layer does not have to pass events on to the engine,
	// since GraphicsScene does that.  Need to think about if this is the
	// best way to do it.

protected:

	// The engine that belongs to the layer.
	std::unique_ptr<EngineType> m_engine = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//