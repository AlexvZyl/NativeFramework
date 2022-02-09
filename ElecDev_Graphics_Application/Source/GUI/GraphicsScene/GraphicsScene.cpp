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
	: GuiElementCore(name, windowFlags | ImGuiWindowFlags_NoScrollbar)
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
	// Pass events to gui element.
	GuiElementCore::onEvent(event);

	// The engine must be aware of these events, even if it is closed.
	uint64_t eventID = event.ID;
	if (eventID == EventType_Hover || eventID == EventType_Dehover || eventID == EventType_Focus || eventID == EventType_Defocus)
		m_engine->onEvent(event);

	// Do not pass events to the engine in these cases.
	else if (m_isCollapsed || !m_isOpen || m_isHidden) return;

	// Pass events to the engine.
	m_engine->onEvent(event);
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

EngineCore* GraphicsScene::getEngine() 
{
	return m_engine.get();
}

void GraphicsScene::begin()
{
	// Setup.
	ImGui::SetNextWindowSize(ImVec2(400,400), ImGuiCond_Once);

	// Remove window padding.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.f, 1.f));
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void GraphicsScene::renderBody() 
{
	// Should not render the engine in these cases.
	if (m_isCollapsed || !m_isOpen || m_isHidden) return;

	m_engine->onRender();
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
