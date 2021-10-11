/*
This is where the drawing enigine mouse events are handled.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "designEngineCore.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Press event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse left click.
void DesignEngineGL::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	BaseEngineGL::mousePressLeft(pixelCoords);
}

// Event handler for a mouse right click.
void DesignEngineGL::mousePressRight()
{
	// Call base engine event.
	BaseEngineGL::mousePressRight();
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Move event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse move event.
void DesignEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonState)
{
	// Call base engine event.
	BaseEngineGL::mouseMoveEvent(pixelCoords, buttonState);

	// Update the mouse point.
	m_mousePoint->updatePosition(pixelCoords);
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Scroll event.
//----------------------------------------------------------------------------------------------------------------------

void DesignEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Call the base engine event.
	BaseEngineGL::mouseScrollEvent(pixelCoords, yOffset);
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------