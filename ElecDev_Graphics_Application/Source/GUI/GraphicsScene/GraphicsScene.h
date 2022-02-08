#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include "GUI/GuiElementCore/GuiElementCore.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EngineCore;

//==============================================================================================================================================//
//  Graphics Scene.																																//
//==============================================================================================================================================//

class GraphicsScene : public GuiElementCore
{
public:

	// Constructor.
	GraphicsScene(uint64_t ID, std::string name, int windowFlags);

	// Rendering functions.
	virtual void begin() override;
	virtual void renderBody() override;
	virtual void end() override;

	// Get the engine related to the element.
	EngineCore* getEngine();

	// Overridden to be able to call engine events.
	virtual void onEvent(Event& event) override;

private:

	// Rendering texture data.
	void* m_textureID = nullptr;

	// The engine that is in this layer.
	std::unique_ptr<EngineCore> m_engine = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
