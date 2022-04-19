//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Application/Events/Events.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Lumen.h"
#include "Application/Application.h"
#include "GUI/PopUpMenu/PopUpMenu.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

void Base2DEngine::onMouseButtonEvent(const MouseButtonEvent& event)
{
	// --------------------- //
	//  L E F T   P R E S S  //
	// --------------------- //

	if (event.isType(EventType_MousePress | EventType_MouseButtonLeft))
	{
		// Find current click in world coords.
		glm::vec3 m_prevMouseEventWorldVec = pixelCoordsToCameraCoords(event.mousePosition);
	}

	// ----------------------- //
	//  R I G H T   P R E S S  //
	// ----------------------- //

	else if (event.isType(EventType_MousePress | EventType_MouseButtonRight))
	{
		
	}

	// ------------------------- //
	//  M I D D L E   P R E S S  //
	// ------------------------- //

	else if (event.isType(EventType_MousePress | EventType_MouseButtonMiddle))
	{
		// Find current click in world coords.
		m_prevMouseEventWorldVec = pixelCoordsToCameraCoords(event.mousePosition);
	}
}

void Base2DEngine::onMouseMoveEvent(const MouseMoveEvent& event)
{
	EngineCore::onMouseMoveEvent(event);

	// Find current click in world coords.
	glm::vec3 currmousePosVec = pixelCoordsToCameraCoords(event.mousePosition);

	// ------------------------- //
	//  M I D D L E   P R E S S  //
	// ------------------------- //

	if (event.isType(EventType_MouseButtonMiddle))
	{
		// Calculate distance to translate
		glm::vec3 translateVec({ (currmousePosVec.x - m_prevMouseEventWorldVec.x),(currmousePosVec.y - m_prevMouseEventWorldVec.y), 0 });
		// Translate to the view matrix.
		m_scene->m_camera->m_translationMatrix = glm::translate(m_scene->m_camera->m_translationMatrix, translateVec);
	}
	
	// --------- //
	//  D A T A  //
	// --------- //

	// Save mouse click position.
	m_prevMouseEventWorldVec = currmousePosVec;
}

void Base2DEngine::onMouseScrollEvent(const MouseScrollEvent& event)
{
	// Calculate zoom value based on mouse wheel scroll.
	//----------------------------------------------------------
	// Define the scaling value.
	static float zoomScaleValue;
	// Zoom in.
	if (event.yOffset > 0) { zoomScaleValue = 1 + m_scene->m_camera->m_scaleRate; }
	// Zoom out.
	else { zoomScaleValue = 1 / (1 + (m_scene->m_camera->m_scaleRate)); }

	//  Apply scale and translate to keep zoom around mouse.
	//----------------------------------------------------------
	// Calculate mouse world coords before scaling.
	glm::vec3 mouseWorldCoordsPre = pixelCoordsToCameraCoords(event.mousePosition);
	// Apply scaling.
	glm::vec3 scaleVector = glm::vec3(zoomScaleValue, zoomScaleValue, 1);
	m_scene->m_camera->m_scalingMatrix = glm::scale(m_scene->m_camera->m_scalingMatrix, scaleVector);
	// Calculate mouse world coordinates after scaling.
	glm::vec3 mouseWorldCoordsPost = pixelCoordsToCameraCoords(event.mousePosition);
	// Translate the world so that the zoom happens on the mouse position. 
	glm::vec3 translateVector = glm::vec3(mouseWorldCoordsPost.x - mouseWorldCoordsPre.x, mouseWorldCoordsPost.y - mouseWorldCoordsPre.y, 0);
	//  Apply translation vector.
	m_scene->m_camera->m_translationMatrix = glm::translate(m_scene->m_camera->m_translationMatrix, translateVector);

	// Update mouse coordinates.
	m_prevMouseEventWorldVec = mouseWorldCoordsPost;
}

//==============================================================================================================================================//
//  Key Events.																																	//
//==============================================================================================================================================//

void Base2DEngine::onKeyEvent(const KeyEvent& event) 
{

}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//