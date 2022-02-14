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
GraphicsScene::GraphicsScene(std::string name, int windowFlags)
	: GuiElementCore(name, windowFlags | ImGuiWindowFlags_NoScrollbar)
{}

void GraphicsScene::setEngine(EngineCore* engine) 
{
	m_engine = engine;
	m_textureID = (void*)m_engine->getRenderTexture();
}

EngineCore* GraphicsScene::getEngine() 
{
	return m_engine;
}

void GraphicsScene::onEvent(Event& event) 
{
	// Pass evevnts to the GUI.
	GuiElementCore::onEvent(event);

	// We need to pass events on to the engine as well.
	m_engine->onEvent(event);
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

void GraphicsScene::begin()
{
	// Setup.
	ImGui::SetNextWindowSize(ImVec2(400,400), ImGuiCond_Once);

	// Adjust window padding.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.f, 1.f));
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void GraphicsScene::onRender() 
{
	ImGui::Image(m_textureID, m_contentRegionSize, ImVec2(0, 1), ImVec2(1, 0));
}

void GraphicsScene::end()
{
	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
