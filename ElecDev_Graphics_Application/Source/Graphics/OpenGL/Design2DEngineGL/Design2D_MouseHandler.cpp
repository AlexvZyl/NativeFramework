/*
This is where the drawing enigine mouse events are handled.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "Design2D_Engine.h"

//=============================================================================================================================================//
//  Press event.																															   //
//=============================================================================================================================================//

// Event handler for a mouse left click.
void Design2DEngineGL::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressLeft(pixelCoords);
	m_activeComponent = std::make_shared<Component2D>();
	m_activeComponent->moveTo(pixelCoords);
	m_components.insert(m_components.end(), m_activeComponent);
}

// Event handler for a mouse right click.
void Design2DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressRight(pixelCoords);
}

//=============================================================================================================================================//
//  Move event.																																   //
//=============================================================================================================================================//

// Event handler for a mouse move event.
void Design2DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight)
{
	// Move the component.
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
	m_activeComponent->moveTo(screenCoords);
	m_activeComponent->draw();

	// Call parent event.
	Base2DEngineGL::mouseMoveEvent(pixelCoords, buttonStateLeft, buttonStateRight);
}

//=============================================================================================================================================//
//  Scroll event.																															   //
//=============================================================================================================================================//

void Design2DEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Call the base engine event.
	Base2DEngineGL::mouseScrollEvent(pixelCoords, yOffset);

	// Move the component.
	glm::vec3 WorldCoords = pixelCoordsToWorldCoords(pixelCoords);
	float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
	m_activeComponent->moveTo(screenCoords);
	m_activeComponent->draw();
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//