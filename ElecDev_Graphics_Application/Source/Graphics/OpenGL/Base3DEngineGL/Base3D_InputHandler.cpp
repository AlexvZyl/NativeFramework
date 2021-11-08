//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base3D_Engine.h"
#include <GLFW/glfw3.h>

//=============================================================================================================================================//
//  Mouse presses.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::mousePressLeft(float pixelCoords[2]) 
{
	// Store the new mouse pixel coordinates.
	m_prevMouseEventPixelCoords[0] = pixelCoords[0];
	m_prevMouseEventPixelCoords[1] = pixelCoords[1];
	// Store previous mouse event world vector.
	glm::vec3 mouseWorldCoords = pixelCoordsToCameraCoords(pixelCoords);
	m_prevMouseEventWorldVec[0] = mouseWorldCoords[0];
	m_prevMouseEventWorldVec[1] = mouseWorldCoords[1];
	m_prevMouseEventWorldVec[2] = mouseWorldCoords[2];
}

void Base3DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Store the new mouse pixel coordinates.
	m_prevMouseEventPixelCoords[0] = pixelCoords[0];
	m_prevMouseEventPixelCoords[1] = pixelCoords[1];
	// Store previous mouse event world vector.
	glm::vec3 mouseWorldCoords = pixelCoordsToCameraCoords(pixelCoords);
	m_prevMouseEventWorldVec[0] = mouseWorldCoords[0];
	m_prevMouseEventWorldVec[1] = mouseWorldCoords[1];
	m_prevMouseEventWorldVec[2] = mouseWorldCoords[2];
}

void Base3DEngineGL::mousePressMiddle(float pixelCoords[2])
{
	// Store the new mouse pixel coordinates.
	m_prevMouseEventPixelCoords[0] = pixelCoords[0];
	m_prevMouseEventPixelCoords[1] = pixelCoords[1];
	// Store previous mouse event world vector.
	glm::vec3 mouseWorldCoords = pixelCoordsToCameraCoords(pixelCoords);
	m_prevMouseEventWorldVec[0] = mouseWorldCoords[0];
	m_prevMouseEventWorldVec[1] = mouseWorldCoords[1];
	m_prevMouseEventWorldVec[2] = mouseWorldCoords[2];
}

//=============================================================================================================================================//
//  Mouse movement.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle)
{	
	// Implement rotation.
	if (buttonStateLeft == GLFW_PRESS) 
	{
		// Calculate the change in mouse movement.
		float mouseMovementPixels[2] = {pixelCoords[0]-m_prevMouseEventPixelCoords[0], pixelCoords[1] - m_prevMouseEventPixelCoords[1] };
		m_camera->rotateAroundTarget(mouseMovementPixels);
		// Store the new mouse pixel coordinates.
		m_prevMouseEventPixelCoords[0] = pixelCoords[0];
		m_prevMouseEventPixelCoords[1] = pixelCoords[1];
		// Store previous mouse event world vector.
		glm::vec3 mouseWorldCoords = pixelCoordsToCameraCoords(pixelCoords);
		m_prevMouseEventWorldVec[0] = mouseWorldCoords[0];
		m_prevMouseEventWorldVec[1] = mouseWorldCoords[1];
		m_prevMouseEventWorldVec[2] = mouseWorldCoords[2];
	}
	// Implement panning.
	if (buttonStateMiddle == GLFW_PRESS)
	{	
		// Calculate the change in mouse movement.
		float mouseMovementWorld[2] = { (pixelCoords[0] - m_prevMouseEventPixelCoords[0]), (pixelCoords[1] - m_prevMouseEventPixelCoords[1]) };
		m_camera->panCamera(mouseMovementWorld);
		// Store the new mouse pixel coordinates.
		m_prevMouseEventPixelCoords[0] = pixelCoords[0];
		m_prevMouseEventPixelCoords[1] = pixelCoords[1];
		// Store previous mouse event world vector.
		glm::vec3 mouseWorldCoords = pixelCoordsToCameraCoords(pixelCoords);
		m_prevMouseEventWorldVec[0] = mouseWorldCoords[0];
		m_prevMouseEventWorldVec[1] = mouseWorldCoords[1];
		m_prevMouseEventWorldVec[2] = mouseWorldCoords[2];
	}
}

//=============================================================================================================================================//
//  Mouse scrolling.																														   //
//=============================================================================================================================================//

void Base3DEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Calculate mouse world coordinates before moving forward..
	glm::vec3 mouseWorldCoordsPre = pixelCoordsToCameraCoords(pixelCoords);
	// Move in the direction of the target on a mouse scroll and update the view matrix.
	m_camera->translateTowardsTarget(yOffset);
	m_camera->updateView();
	// Calculate mouse world coordinates after moving forward.
	glm::vec3 mouseWorldCoordsPost = pixelCoordsToCameraCoords(pixelCoords);
	// Translate the world so that the zoom happens on the mouse position. 
	float translateVector[3] = { (float)mouseWorldCoordsPost[0] - (float)mouseWorldCoordsPre[0], (float)mouseWorldCoordsPost[1] - (float)mouseWorldCoordsPre[1], 0.0f };
	//  Apply translation vector.
	m_camera->translateTowardsTarget(yOffset * 0.5f);
	m_prevMouseEventPixelCoords[0] = pixelCoords[0];
	m_prevMouseEventPixelCoords[1] = pixelCoords[1];
}

//=============================================================================================================================================//
//  Key press.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::keyPressEvent(int key) 
{

}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//
