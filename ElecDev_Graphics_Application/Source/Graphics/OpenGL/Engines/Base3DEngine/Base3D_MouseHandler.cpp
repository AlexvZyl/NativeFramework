//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Base3D_Engine.h"

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

void Base3DEngineGL::mousePressLeft(float pixelCoords[2]) 
{

}

void Base3DEngineGL::mousePressRight() 
{
	BaseEngineGL::mousePressRight();
}

void Base3DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonState) 
{	
	BaseEngineGL::mouseMoveEvent(pixelCoords, buttonState);

	// Implement rotation.
	if (buttonState == GLFW_PRESS) 
	{
		//m_rotationMatrix = glm::rotate(m_rotationMatrix, );
	}

}

void Base3DEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset) 
{
	m_translationMatrix = glm::translate(m_translationMatrix, glm::vec3(0.0f, 0.0f, yOffset*0.5f));
}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//