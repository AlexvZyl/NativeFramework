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
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Application/Application.h"
#include "Lumen.h"

//==============================================================================================================================================//
//  Constructor.																																//
//==============================================================================================================================================//

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

void GraphicsScene::onRenderStateChange(bool newState)
{
	// Need to start using the attachments.
	if (newState)
	{
		m_engine->m_scene->createFrameBufferResources();
		// Reassign FBO.
		m_textureID = (void*) m_engine->getRenderTexture();
	}

	// Have no use for the attachments.
	else
	{
		m_engine->m_scene->deleteFrameBufferResources();
		// The FBO has been destroyed.
		m_textureID = nullptr;
	}
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void GraphicsScene::onContentRegionMoveEvent(WindowEvent& event) 
{
	GuiElementCore::onContentRegionMoveEvent(event);

	// Pass data to engine.
	m_engine->setContentRegionPos(glm::vec2(event.windowData.x, event.windowData.y));
}

void GraphicsScene::onContentRegionResizeEvent(WindowEvent& event) 
{
	GuiElementCore::onContentRegionResizeEvent(event);

	// Pass data to engine.
	m_engine->setContentRegionSize(glm::vec2(event.windowData.x, event.windowData.y));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
