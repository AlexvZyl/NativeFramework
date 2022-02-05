//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Events/Events.h"
#include "External/Misc/ConsoleColor.h"
#include <iostream>
#include "Layer.h"
#include "Engines/EngineCore/EngineCore.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Engines/Base3DEngine/Base3DEngine.h"
#include "GUI/GuiElementCore/GuiElementCore.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/CameraGL.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "OpenGL/RendererGL.h"

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

Layer::Layer(uint64_t ID, std::string layerName) 
	: m_layerID(ID), m_layerName(layerName)
{}

void Layer::setBorder(ImVec2& min, ImVec2& max) 
{
	m_borderMin = min;
	m_borderMax = max;
}

bool Layer::isMouseHovering()
{
	// We should check how the clipping works in ImGui.
	return ImGui::IsMouseHoveringRect(m_borderMin, m_borderMax, false);
}

void Layer::setDockingState(bool state)
{
	// On state change.
	if (m_docked != state)
	{
		onDockingStateChange(state);
		m_docked = state;
	}
}

void Layer::onDockingStateChange(bool newState) 
{
	// If docked, get the dock ID.
	if (newState) m_dockID = ImGui::GetWindowDockID();

	// The layer should be moved (in the stack) based on where it has been docked/undocked.
}

uint64_t Layer::getID() 
{
	return m_layerID;
}

void Layer::updateLayerData()
{
	// See if the layer has resized.
	detectResize();

	// Border.
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 borderMax = { windowPos.x + m_layerSize.x, windowPos.y + m_layerSize.y };
	setBorder(windowPos, borderMax);

	// Docking.
	setDockingState(ImGui::IsWindowDocked());
}

void Layer::detectResize() 
{
	ImVec2 currentSize = ImGui::GetWindowSize();
	if (currentSize.x != m_layerSize.x || currentSize.y != m_layerSize.y)
	{
		glm::vec2 eventSize = { currentSize.x, currentSize.y };
		WindowResizeEvent event(eventSize, 0);
		onEvent(event);
		m_layerSize = currentSize;
	}
}

//==============================================================================================================================================//
//  GUI Layer.																																	//
//==============================================================================================================================================//

GuiLayer::GuiLayer(uint64_t ID, std::string layerName, int imguiWindowFlags)
	: Layer(ID | LayerType_GUI, layerName), m_imguiWindowflags(imguiWindowFlags)
{
	if (ID == LayerType_ComponentEditor)
	{
		m_guiElement = std::make_unique<ComponentEditor>(m_layerName, m_imguiWindowflags);
	}
	else
	{
		std::cout << red << "\n[LAYERS] [ERROR]: " << white << "Could not create GuiLayer based on the provided ID.\n";
		assert(true);
	}
}

GuiElementCore* GuiLayer::getGuiElement() 
{
	return m_guiElement.get();
}

void GuiLayer::onEvent(Event& event)
{
	m_guiElement->onEvent(event); 
	std::cout << m_guiElement->m_name << ": " << event.getID() << "\n";
}

void GuiLayer::onRender() 
{
	// Setup.
	m_guiElement->begin();

	// Update the layer data.
	updateLayerData();

	// Render the ImGUI window.
	m_guiElement->renderBody();

	// Cleanup.
	m_guiElement->end(); 
}

//==============================================================================================================================================//
//  Engine Layer.																																//
//==============================================================================================================================================//

EngineLayer::EngineLayer(uint64_t ID, std::string layerName, int imguiWindowFlags) 
	: Layer(ID | LayerType_Engine, layerName), m_imguiWindowflags(imguiWindowFlags)
{
	// Create the engine component.
	if		(ID == LayerType_Base2DEngine)	 m_engine = std::make_unique<Base2DEngine>();
	else if (ID == LayerType_Base3DEngine)	 m_engine = std::make_unique<Base3DEngine>();
	else if (ID == LayerType_Design2DEngine) m_engine = std::make_unique<Design2DEngine>();
	else if (ID == LayerType_Design3DEngine) m_engine = nullptr;
	// Error.
	else
	{
		std::cout << red << "\n[LAYERS] [ERROR]: " << white << "Could not create EngineLayer based on the provided ID.\n";
		assert(true);
	}

	// Set the render texture.
	m_imguiWindowflags |= ImGuiWindowFlags_NoScrollbar;
	m_guiElement = std::make_unique<GraphicsScene>(m_layerName, m_imguiWindowflags);
	m_guiElement->setTextureID(m_engine->getRenderTexture());

	Renderer::bindScene(m_engine->m_scene.get());
	glm::vec2 center(0.f, 0.f);
	glm::vec4 black(1.f, 0.f, 1.f, 1.f);
	Renderer::addCircle2D(center, 0.5, black);
}

EngineCore* EngineLayer::getEngine() 
{
	return m_engine.get();
}

GuiElementCore* EngineLayer::getGuiElement() 
{
	return m_guiElement.get();
}

void EngineLayer::onEvent(Event& event) 
{
	m_engine->onEvent(event);
	std::cout << m_guiElement->m_name << ": " << event.getID() << "\n";
}

void EngineLayer::onRender() 
{
	// Setup.
	m_guiElement->begin();

	// Update the layer data.
	updateLayerData();

	// Render the engine scene.
	m_engine->onRender();

	// Render the ImGUI window.
	m_guiElement->renderBody();

	// Cleanup.
	m_guiElement->end();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
