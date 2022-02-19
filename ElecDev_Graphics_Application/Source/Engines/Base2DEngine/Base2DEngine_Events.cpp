//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Engines/Base2DEngine/Base2DEngine.h"
#include "Application/Events/Events.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Lumen.h"
#include "Application/Application.h"
#include "GUI/PopUpMenu/PopUpMenu.h"

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

void Base2DEngine::onMouseButtonEvent(MouseButtonEvent& event)
{
	uint64_t eventID = event.ID;

	// --------------------- //
	//  L E F T   P R E S S  //
	// --------------------- //

	if (eventID == (EventType_MousePress | EventType_MouseButtonLeft))
	{
		// Find current click in world coords.
		glm::vec2 pos = event.mousePosition;
		float posf[2] = {pos.x, pos.y};
		glm::vec3 currmousePosVec = m_scene->pixelCoordsToCameraCoords(posf);
		// Save current mouse pos click.
		m_prevMouseEventWorldVec[0] = currmousePosVec[0];
		m_prevMouseEventWorldVec[1] = currmousePosVec[1];
	}

	// ----------------------- //
	//  R I G H T   P R E S S  //
	// ----------------------- //

	else if (eventID == (EventType_MousePress | EventType_MouseButtonRight))
	{
		
	}

	// ------------------------- //
	//  M I D D L E   P R E S S  //
	// ------------------------- //

	else if (eventID == (EventType_MousePress | EventType_MouseButtonMiddle))
	{
		// Find current click in world coords.
		glm::vec2 pos = event.mousePosition;
		float posf[2] = { pos.x, pos.y };
		glm::vec3 currmousePosVec = m_scene->pixelCoordsToCameraCoords(posf);
		// Save current mouse pos click.
		m_prevMouseEventWorldVec[0] = currmousePosVec[0];
		m_prevMouseEventWorldVec[1] = currmousePosVec[1];
	}
}

void Base2DEngine::onMouseMoveEvent(MouseMoveEvent& event)
{
	uint64_t eventID = event.ID;

	// Find current click in world coords.
	glm::vec2 pos = event.mousePosition;
	float posf[2] = { pos.x, pos.y };
	glm::vec3 currmousePosVec = m_scene->pixelCoordsToCameraCoords(posf);

	// ------------------------- //
	//  M I D D L E   P R E S S  //
	// ------------------------- //

	if (eventID == EventType_MouseButtonMiddle)
	{
		// Calculate distance to translate
		glm::vec3 translateVec({ (currmousePosVec[0] - m_prevMouseEventWorldVec[0]),(currmousePosVec[1] - m_prevMouseEventWorldVec[1]), 0 });
		// Translate to the view matrix.
		m_scene->m_camera->m_translationMatrix = glm::translate(m_scene->m_camera->m_translationMatrix, translateVec);
	}
	
	// --------- //
	//  D A T A  //
	// --------- //

	// Save mouse click position.
	m_prevMouseEventWorldVec[0] = currmousePosVec[0];
	m_prevMouseEventWorldVec[1] = currmousePosVec[1];
}

void Base2DEngine::onMouseScrollEvent(MouseScrollEvent& event)
{
	// Calculate zoom value based on mouse wheel scroll.
	//----------------------------------------------------------
	// Define the scaling value.
	float zoomScaleValue;
	// Zoom in.
	if (event.yOffset > 0) { zoomScaleValue = 1 + m_scene->m_camera->m_scaleRate; }
	// Zoom out.
	else { zoomScaleValue = 1 / (1 + (m_scene->m_camera->m_scaleRate)); }

	//  Apply scale and translate to keep zoom around mouse.
	//----------------------------------------------------------
	// Calculate mouse world coords before scaling.
	glm::vec2 pos = event.mousePosition;
	float posf[2] = { pos.x, pos.y };
	glm::vec3 mouseWorldCoordsPre = m_scene->pixelCoordsToCameraCoords(posf);
	// Apply scaling.
	glm::vec3 scaleVector = glm::vec3(zoomScaleValue, zoomScaleValue, 1);
	m_scene->m_camera->m_scalingMatrix = glm::scale(m_scene->m_camera->m_scalingMatrix, scaleVector);
	// Calculate mouse world coordinates after scaling.
	glm::vec3 mouseWorldCoordsPost = m_scene->pixelCoordsToCameraCoords(posf);
	// Translate the world so that the zoom happens on the mouse position. 
	glm::vec3 translateVector = glm::vec3((float)mouseWorldCoordsPost[0] - (float)mouseWorldCoordsPre[0], (float)mouseWorldCoordsPost[1] - (float)mouseWorldCoordsPre[1], 0);
	//  Apply translation vector.
	m_scene->m_camera->m_translationMatrix = glm::translate(m_scene->m_camera->m_translationMatrix, translateVector);

	// Update mouse coordinates.
	m_prevMouseEventWorldVec[0] = mouseWorldCoordsPost[0];
	m_prevMouseEventWorldVec[1] = mouseWorldCoordsPost[1];
}

//==============================================================================================================================================//
//  Key Events.																																	//
//==============================================================================================================================================//

void Base2DEngine::onKeyEvent(KeyEvent& event) 
{

}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//