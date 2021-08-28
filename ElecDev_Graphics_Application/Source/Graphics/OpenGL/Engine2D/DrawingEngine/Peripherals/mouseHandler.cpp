/*
This is where the drawing enigine mouse events are handled.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Engine2D/DrawingEngine/drawingEngine.h"

// Error handler.
#include <ErrorHandler/errorHandler.h>
#include "../Helper/stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Press event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse left click.
void DrawingEngineGL::mousePressLeft(double pixelCoords[2])
{
	// Find current click in world coords.
	glm::vec4 currmousePosVec = this->pixelCoordsToWorldCoords(pixelCoords);
	// Save current mouse pos click.
	this->prevMouseEventWorldCoords[0] = currmousePosVec[0];
	this->prevMouseEventWorldCoords[1] = currmousePosVec[1];
}

// Event handler for a mouse right click.
void DrawingEngineGL::mousePressRight(double pixelCoords[2])
{
	// Reset the view values.
	this->scalingMatrix = glm::mat4(1.0f);
	this->translationMatrix = glm::mat4(1.0f);
	this->rotationMatrix = glm::mat4(1.0f);
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Move event.
//----------------------------------------------------------------------------------------------------------------------

// Event handler for a mouse move event.
void DrawingEngineGL::mouseMoveEvent(double pixelCoords[2], int buttonState)
{
	// Check if left mouse is pressed.
	if (buttonState == GLFW_PRESS)
	{
		// Find current mouse position in the world.
		glm::vec4 currMousePosVec = this->pixelCoordsToWorldCoords(pixelCoords);
		// Calculate distance to translate.
		glm::vec3 translateVec({ (currMousePosVec[0] - this->prevMouseEventWorldCoords[0]),(currMousePosVec[1] - this->prevMouseEventWorldCoords[1]),0 });
		// Translate to the view matrix.
		this->translationMatrix = glm::translate(this->translationMatrix, translateVec);

		// Save mouse click position.
		this->prevMouseEventWorldCoords[0] = currMousePosVec[0];
		this->prevMouseEventWorldCoords[1] = currMousePosVec[1];
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse Scroll event.
//----------------------------------------------------------------------------------------------------------------------

void DrawingEngineGL::mouseScrollEvent(double pixelCoords[2], int yOffset) 
{	
	
	// Calculate zoom value based on mouse wheel scroll.
	//----------------------------------------------------------
	
	// Define the scaling value.
	double zoomScaleValue;
	 
	// Determine if it should be a zoom in or out.
	if (yOffset > 0) // Zoom in.
	{	
		// Zoom scale value.
		zoomScaleValue = 1 + (double)this->scaleRate;
	}
	else // Zoom out.
	{
		zoomScaleValue = 1 / (1 + (double)this->scaleRate);
	}

	//  Apply scale and translate to keep zoom around mouse.
	//----------------------------------------------------------
	
	// Calculate mouse world coords before scaling.
	glm::vec4 mouseWorldCoordsPre = this->pixelCoordsToWorldCoords(pixelCoords);
	// Apply scaling.
	glm::vec3 scaleVector = glm::vec3(zoomScaleValue, zoomScaleValue, 1);
	this->scalingMatrix = glm::scale(this->scalingMatrix, scaleVector);
	// Calculate mouse world coordinates after scaling.
	glm::vec4 mouseWorldCoordsPost = this->pixelCoordsToWorldCoords(pixelCoords);
	// Translate the world so that the zoom happens on the mouse position. 
	glm::vec3 translateVector = glm::vec3((double)mouseWorldCoordsPost[0] - (double)mouseWorldCoordsPre[0], (double)mouseWorldCoordsPost[1] - (double)mouseWorldCoordsPre[1], 0);
	// Adjust translate vector for when zooming out.
	if (yOffset < 0) 
	{ 
		translateVector = { -1 * translateVector[0], translateVector[1], 0 };
	}
	//  Apply translation vector.
	this->translationMatrix = glm::translate(this->translationMatrix, translateVector);
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------