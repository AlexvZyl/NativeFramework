//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GraphicsScene.h"
#include "External/ImGUI/Core/imgui.h"
#include "Application/Events/Events.h"
#include "Application/Layers/Layer.h"
#include "Engines/EngineCore/EngineCore.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Engines/Base3DEngine/Base3DEngine.h"
#include "Engines/Design2DEngine/Design2DEngine.h"

//==============================================================================================================================================//
//  Constructor.																																//
//==============================================================================================================================================//

// Constructor.
GraphicsScene::GraphicsScene(uint64_t ID, std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags)
{
	// Create the engine component.
	if		(ID == LayerType_Base2DEngine)	 m_engine = std::make_unique<Base2DEngine>();
	else if (ID == LayerType_Base3DEngine)	 m_engine = std::make_unique<Base3DEngine>();
	else if (ID == LayerType_Design2DEngine) m_engine = std::make_unique<Design2DEngine>();
	else if (ID == LayerType_Design3DEngine) m_engine = nullptr;

	// Set the texture to be rendred.
	m_textureID = (void*)m_engine->getRenderTexture();
}

//==============================================================================================================================================//
//  EVents.																																		//
//==============================================================================================================================================//

void GraphicsScene::onEvent(Event& event) 
{
	GuiElementCore::onEvent(event);
	m_engine->onEvent(event);
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

EngineCore* GraphicsScene::getEngine() 
{
	return m_engine.get();
}

void GraphicsScene::renderBody() 
{
	m_engine->onRender();
	ImGui::Image(m_textureID, m_contentRegionSize, ImVec2(0, 1), ImVec2(1, 0));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
