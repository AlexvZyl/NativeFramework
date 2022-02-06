//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "Application/Layers/EngineLayer.h"
#include "Application/Events/Events.h"
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Engines/Base3DEngine/Base3DEngine.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "GUI/GraphicsScene/GraphicsScene.h"

// Testing.
#include "OpenGL/RendererGL.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Engine Layer.																																//
//==============================================================================================================================================//

EngineLayer::EngineLayer(LayerType engineType, std::string layerName, int imguiWindowFlags)
	: Layer(engineType | LayerType_Engine, layerName), m_imguiWindowflags(imguiWindowFlags)
{
	// Create the engine component.
	if	    (engineType == LayerType_Base2DEngine  ) m_engine = std::make_unique<Base2DEngine>();
	else if (engineType == LayerType_Base3DEngine  ) m_engine = std::make_unique<Base3DEngine>();
	else if (engineType == LayerType_Design2DEngine) m_engine = std::make_unique<Design2DEngine>();
	else if (engineType == LayerType_Design3DEngine) m_engine = nullptr;

	// Error.
	else
	{
		std::cout << red << "\n[LAYERS] [ERROR]: " << white << "Could not create EngineLayer based on the provided ID.\n";
		assert(true);
	}

	// Create an ImGui window to render the engine to.
	m_imguiWindowflags |= ImGuiWindowFlags_NoScrollbar;
	m_graphicsWindow = std::make_unique<GraphicsScene>(m_layerName, m_imguiWindowflags);
	m_graphicsWindow->setTextureID(m_engine->getRenderTexture());

	Renderer::bindScene(m_engine->m_scene.get());
	glm::vec2 center(0.f, 0.f);
	glm::vec4 pink(1.f, 0.f, 1.f, 1.f);
	Renderer::addCircle2D(center, 0.5, pink);
}

EngineCore* EngineLayer::getEngine()
{
	return m_engine.get();
}

GuiElementCore* EngineLayer::getGuiElement()
{
	return m_graphicsWindow.get();
}

void EngineLayer::onEvent(Event& event)
{
	m_graphicsWindow->onEvent(event);
	m_engine->onEvent(event);
	std::cout << m_graphicsWindow->m_name << ": " << event.getID() << "\n";
}

void EngineLayer::onRender()
{
	m_graphicsWindow->begin();
	updateLayerData();
	m_engine->onRender();
	m_graphicsWindow->renderBody();
	m_graphicsWindow->end();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
