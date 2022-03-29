//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GraphicsScene.h"
#include "imgui/imgui.h"
#include "Application/Events/Events.h"
#include "Application/Layers/Layer.h"
#include "Engines/EngineCore/EngineCore.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Engines/Base3DEngine/Base3DEngine.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Application/Application.h"
#include "Lumen.h"
#include "OpenGL/Renderer/RendererGL.h"

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
	// Adjust window padding.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void GraphicsScene::onRender() 
{
	m_engine->onRender();
	if (!m_textureID) return;
	ImGui::Image(m_textureID, m_contentRegionSize, ImVec2(0, 1), ImVec2(1, 0));
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) 
		{
			// Pass FileDropEvent to engine.
			const wchar_t* path = (const wchar_t*)payload->Data;
			std::wstring wPath(path);
			std::string sPath;
			sPath.insert(sPath.end(), wPath.begin(), wPath.end());
			FileDropEvent event(sPath);
			Lumen::getApp().getActiveEngine()->onEvent(event);
			ImGui::EndDragDropTarget();
		}
	}
}

void GraphicsScene::end()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

void GraphicsScene::onRenderStateChange(bool newState)
{
	// Create FBO resources.
	if (newState)
	{
		m_engine->m_scene->recreateGPUResources();
		m_textureID = (void*)m_engine->getRenderTexture();
	}

	// Destroy FBO resources.
	else
	{
		m_engine->m_scene->deleteGPUResources();
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
