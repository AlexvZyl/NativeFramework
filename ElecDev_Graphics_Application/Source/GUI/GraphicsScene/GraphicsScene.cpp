//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GraphicsScene.h"
#include "imgui/imgui.h"
#include "Application/Events/Events.h"
#include "Application/LumenWindow/LumenWindow.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "Application/Application.h"
#include "Lumen.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Application/Events/EventLog.h"

//==============================================================================================================================================//
//  Constructor.																																//
//==============================================================================================================================================//

void GraphicsScene::onRender() 
{

}

void GraphicsScene::end()
{

}

void GraphicsScene::onRenderStateChange(bool newState)
{

}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void GraphicsScene::onContentRegionMoveEvent(WindowEvent& event) 
{
	LumenWindowCore::onContentRegionMoveEvent(event);

	// Pass data to engine.
	m_engine->setContentRegionPos(glm::vec2(event.windowData.x, event.windowData.y));
}

void GraphicsScene::onContentRegionResizeEvent(WindowEvent& event) 
{
	// Adjust the event so that the scene fits correctly.
	LumenWindowCore::onContentRegionResizeEvent(event);

	// Pass data to engine.
	WindowEvent resizeEvent({ event.windowData.x, event.windowData.y}, EventType_WindowResize);
	m_engine->onEvent(resizeEvent);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
