#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Engines/EngineCore/EngineCore.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EngineCore;
class Event;

//==============================================================================================================================================//
//  Graphics Scene.																																//
//==============================================================================================================================================//

class GraphicsScene : public GuiElementCore
{
public:

	// Constructor.
	GraphicsScene(std::string name, int windowFlags);
	// Destructor.
	virtual ~GraphicsScene() = default;

	// Rendering functions.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

	// Override the on event so that we pass it to the engine.
	virtual void onEvent(Event& event) override;

	// Event handler for when the render state changes.
	virtual void onRenderStateChange(bool newState) override;

	// Set the texture that the graphics window will render.
	void setEngine(EngineCore* engine);
	// Get the engine inside the graphics scene.
	EngineCore* getEngine();

private:

	// Rendering texture data.
	void* m_textureID = nullptr;
	// The engine that belongs to the window.
	EngineCore* m_engine = nullptr;

	// Mouse events.
	virtual void onMouseButtonEvent(MouseButtonEvent& event);

	// Content region events.  These need to be passed onto the engine.
	virtual void onContentRegionResizeEvent(WindowEvent& event);
	virtual void onContentRegionMoveEvent(WindowEvent& event);
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
