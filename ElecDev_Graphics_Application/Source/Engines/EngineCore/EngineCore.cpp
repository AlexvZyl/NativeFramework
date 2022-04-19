//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include <memory>
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Engines/EngineCore/EngineCore.h"
#include "Application/Application.h"
#include "Application/LumenWindow/LumenWindow.h"
#include "Lumen.h"
#include "OpenGL/Primitives/Grid.h"

//=============================================================================================================================================//
//  Engine Core.																															   //
//=============================================================================================================================================//

void EngineCore::onRender()
{
	Renderer::renderScene(m_scene.get());
}

EngineCore::~EngineCore() 
{
	Application& app = Lumen::getApp();
	if (this == app.getActiveEngine())
		app.setActiveEngine(nullptr);
}

void EngineCore::setName(const std::string& name) 
{
	m_parentWindow->setName(name.c_str());
	setNameOfElements(name); 
}

const glm::vec2& EngineCore::getWindowContentRegionSize() const 
{
	return m_parentWindow->getContentRegionSize();
}

//=============================================================================================================================================//
//  Scene wrappers.																															   //
//=============================================================================================================================================//

Scene& EngineCore::getScene() 
{
	return *m_scene.get();
}

unsigned EngineCore::getRenderTexture() 
{ 
	return m_scene->getRenderTexture(); 
}

unsigned EngineCore::getEntityID(const glm::vec2& pixelCoords) 
{ 
	return m_scene->getEntityID(pixelCoords); 
}

glm::vec2 EngineCore::getNearestGridVertex(const glm::vec2& coords) 
{
	return m_scene->m_grid->getNearestGridVertex(coords);
}

glm::vec2 EngineCore::pixelDistanceToWorldDistance(const glm::vec2& distance)
{
	Camera& camera = m_scene->getCamera();
	return camera.pixelCoordsToWorldCoords(distance) - camera.pixelCoordsToWorldCoords({ 0.f, 0.f });
}

glm::vec3 EngineCore::pixelCoordsToWorldCoords(const glm::vec2& coords) 
{
	return m_scene->getCamera().pixelCoordsToWorldCoords(coords);
}

glm::vec3 EngineCore::pixelCoordsToCameraCoords(const glm::vec2& coords) 
{
	return m_scene->getCamera().pixelCoordsToCameraCoords(coords);
}

//=============================================================================================================================================//
//  Window wrappers.																														   //
//=============================================================================================================================================//

glm::vec2 EngineCore::getMouseLocalPosition()
{
	return m_parentWindow->getMouseLocalPosition();
}

glm::vec2 EngineCore::getMouseGlobalPosition()
{
	return m_parentWindow->getMouseGlobalPosition();
}

void EngineCore::unsavedDocument()
{
	m_parentWindow->unsavedDocument();
}

void EngineCore::savedDocument()
{
	m_parentWindow->savedDocument();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//