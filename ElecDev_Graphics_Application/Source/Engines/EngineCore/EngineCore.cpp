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
#include "GUI/LumenGizmo/LumenGizmo.h"

//=============================================================================================================================================//
//  Engine Core.																															   //
//=============================================================================================================================================//

EngineCore::EngineCore() 
{
	m_lumenGizmo = std::make_unique<LumenGizmo>();
}

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

float EngineCore::getDeltaTime() const 
{
	return Lumen::getApp().getDeltaTime();
}

LumenGizmo& EngineCore::getGizmo() 
{
	return *m_lumenGizmo.get();
}

void EngineCore::hideGizmo() 
{
	getGizmo().hide();
}

void EngineCore::visibleGizmo()
{
	getGizmo().visible();
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
	return getScene().getRenderTexture(); 
}

unsigned EngineCore::getEntityID(const glm::vec2& pixelCoords) 
{ 
	return getScene().getEntityID(pixelCoords); 
}

glm::vec2 EngineCore::getNearestGridVertex(const glm::vec2& coords) 
{
	return getScene().getGrid().getNearestGridVertex(coords);
}

glm::vec3 EngineCore::pixelToWorldDistance(const glm::vec2& distance)
{
	return pixelToWorldCoords(distance) - pixelToWorldCoords({ 0.f, 0.f });
}

glm::vec3 EngineCore::pixelToWorldCoords(const glm::vec2& coords, bool useUpdatedView)
{
	return getScene().getCamera().pixelToWorldCoords(coords, useUpdatedView);
}

glm::vec2 EngineCore::worldToPixelCoords(const glm::vec3& worldCoords, bool useUpdatedView)
{
	return getScene().getCamera().worldToPixelCoords(worldCoords, useUpdatedView);
}

glm::vec2 EngineCore::worldToPixelCoords(const glm::vec2& worldCoords, bool useUpdatedView)
{
	return getScene().getCamera().worldToPixelCoords({ worldCoords, 0.f }, useUpdatedView);
}

glm::vec2 EngineCore::worldToPixelDistance(const glm::vec3& distance)
{
	return worldToPixelCoords(distance) - worldToPixelCoords({ 0.f, 0.f, 0.f });
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

glm::vec2 EngineCore::localToGlobalCoords(const glm::vec2& coords) 
{
	return m_parentWindow->localToGlobalCoords(coords);
}

glm::vec2 EngineCore::globalToLocalCoords(const glm::vec2& coords) 
{
	return m_parentWindow->globalToLocalCoords(coords);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//