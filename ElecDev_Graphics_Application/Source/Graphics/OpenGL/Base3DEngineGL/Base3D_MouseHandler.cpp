//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base3D_Engine.h"

//=============================================================================================================================================//
//  Mouse presses.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::mousePressLeft(float pixelCoords[2]) 
{
	// Store the new mouse pixel coordinates.
	m_prevMouseEventPixelCoords[0] = pixelCoords[0];
	m_prevMouseEventPixelCoords[1] = pixelCoords[1];
}

void Base3DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Store the new mouse pixel coordinates.
	m_prevMouseEventPixelCoords[0] = pixelCoords[0];
	m_prevMouseEventPixelCoords[1] = pixelCoords[1];
}

void Base3DEngineGL::mousePressMiddle(float pixelCoords[2])
{
}

//=============================================================================================================================================//
//  Mouse movement.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight) 
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
	}
	// Implement panning.
	if (buttonStateRight == GLFW_PRESS)
	{	
		// Calculate the change in mouse movement.
		float mouseMovementPixels[2] = { pixelCoords[0] - m_prevMouseEventPixelCoords[0], pixelCoords[1] - m_prevMouseEventPixelCoords[1] };
		m_camera->panCameraPixels(mouseMovementPixels);
		// Store the new mouse pixel coordinates.
		m_prevMouseEventPixelCoords[0] = pixelCoords[0];
		m_prevMouseEventPixelCoords[1] = pixelCoords[1];
	}
}

//=============================================================================================================================================//
//  Mouse scrolling.																														   //
//=============================================================================================================================================//

void Base3DEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Move in the direction of the target on a mouse scroll.
	m_camera->translateTowardsTarget(yOffset);
}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//