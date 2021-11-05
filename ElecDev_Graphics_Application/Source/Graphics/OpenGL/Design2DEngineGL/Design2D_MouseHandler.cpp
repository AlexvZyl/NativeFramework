/*
This is where the drawing enigine mouse events are handled.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Design2D_Engine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Press event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse left click.
void Design2DEngineGL::mousePressLeft(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressLeft(pixelCoords);
}

// Event handler for a mouse right click.
void Design2DEngineGL::mousePressRight(float pixelCoords[2])
{
	// Call base engine event.
	Base2DEngineGL::mousePressRight(pixelCoords);
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Move event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse move event.
void Design2DEngineGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight)
{
	// Call base engine event.

	//std::cout<<getEntityID(pixelCoords)<<std::endl;
	glm::vec4 WorldCoords = Base2DEngineGL::pixelCoordsToWorldCoords(pixelCoords);
	float raw_screenCoords[2];
	Base2DEngineGL::pixelCoordsToScreenCoords(pixelCoords, raw_screenCoords);
	float screenCoords[2] = { WorldCoords[0], WorldCoords[1] };
	std::cout << "Screen = " << raw_screenCoords[0] << ',' << raw_screenCoords[1] << std::endl;
	std::cout << "World = " << screenCoords[0] << ',' << screenCoords[1] << std::endl;

	component->move(screenCoords);
	component->draw();
	Base2DEngineGL::mouseMoveEvent(pixelCoords, buttonStateLeft, buttonStateRight);

	// Update the mouse point position.
	m_mousePoint->updatePosition(pixelCoords);
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Scroll event.
//----------------------------------------------------------------------------------------------------------------------

void Design2DEngineGL::mouseScrollEvent(float pixelCoords[2], float yOffset)
{
	// Call the base engine event.
	Base2DEngineGL::mouseScrollEvent(pixelCoords, yOffset);
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------