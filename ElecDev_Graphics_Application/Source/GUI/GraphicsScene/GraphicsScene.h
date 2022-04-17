#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include "Application/LumenWindow/LumenWindow.h"
#include "Engines/EngineCore/EngineCore.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EngineCore;
class Event;

//==============================================================================================================================================//
//  Graphics Scene.																																//
//==============================================================================================================================================//

class GraphicsScene : public LumenWindow
{
public:

	// Constructor.
	GraphicsScene(std::string name, int windowFlags);
	// Destructor.
	virtual ~GraphicsScene() = default;

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

	// Override the on event so that we pass it to the engine.
	virtual void onEvent(const Event& event) override;

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

	// Content region events.  These need to be passed onto the engine.
	virtual void onWindowResizeEvent(const WindowEvent& event) override;
	virtual void onWindowMoveEvent(const WindowEvent& event) override;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
