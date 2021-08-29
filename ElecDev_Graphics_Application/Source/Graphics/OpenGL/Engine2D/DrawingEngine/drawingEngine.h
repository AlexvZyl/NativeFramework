#pragma once

/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// OpenGL.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shaders/shaderHandler.h>
#include <glm.hpp>

// General.
#include <iostream>

// Error handling.
#include <ErrorHandler/errorHandler.h>
#include "../Helper/stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  The drawingEngine class.
//----------------------------------------------------------------------------------------------------------------------

class DrawingEngineGL
{
public:

	//---------------------------------------------------------------------------------------------------------------------
	// MPV Matrices.
	//---------------------------------------------------------------------------------------------------------------------
	
	// MVP Matrices.
	glm::mat4 modelMatrix;			// The model matrix that places the object in the world.  Is going to be 
									// kept an identity matrix for now.
	glm::mat4 viewMatrix;			// The matrix that handles the camera movement.
									// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 projectionMatrix;		// The matrix that handles the clipping plane (which part of the world is
									// going to be visible to the screen?
	glm::mat4 viewportMatrix;		// The matrix that handles the viewport transform.  Converts screen pixel
									// coordinates to the OpenGL uniform coordinate system.

	// View matrix components.
	glm::mat4 scalingMatrix;		// Handles camera scaling.
	glm::mat4 translationMatrix;	// Handles camera translations.
	glm::mat4 rotationMatrix;		// Handles camera rotations.

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse handler variables.
	//---------------------------------------------------------------------------------------------------------------------

	// Saves the previous mouse event coordinates (in world coordinates).
	double prevMouseEventWorldCoords[2] = {NULL, NULL};

	//---------------------------------------------------------------------------------------------------------------------
	//  Shaders.
	//---------------------------------------------------------------------------------------------------------------------

	Shader basicShader;

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse event variables.
	//---------------------------------------------------------------------------------------------------------------------

	float scaleRate;

	//---------------------------------------------------------------------------------------------------------------------
	//  Buffers.
	//---------------------------------------------------------------------------------------------------------------------
	
	unsigned int VAO;

	//---------------------------------------------------------------------------------------------------------------------
	//  Misc variables.
	//---------------------------------------------------------------------------------------------------------------------

	GLFWwindow* window;
	float viewportDimensions[2];
	float projectionValues[6];

	//---------------------------------------------------------------------------------------------------------------------
	//  Constructors.
	//---------------------------------------------------------------------------------------------------------------------

	// Default.
	DrawingEngineGL();
	// Wiith GLFW window.
	DrawingEngineGL(GLFWwindow* window);
	
	//---------------------------------------------------------------------------------------------------------------------
	//  Rendering.
	//---------------------------------------------------------------------------------------------------------------------

	void renderLoop();

	//---------------------------------------------------------------------------------------------------------------------
	//  API
	//---------------------------------------------------------------------------------------------------------------------

	// Adds a line to the VBO object.
	void drawLine();

	// Adds a circle to the VBO object.
	void drawCircle();

	// Adds text to the VBO object.
	void drawText();

	// Displays the new drawing to the screen.
	// Required after each new element has been added.
	void display();

	//---------------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//---------------------------------------------------------------------------------------------------------------------

	// Event handler mouse presses.
	void mousePressLeft(double pixelCoords[2]);
	void mousePressRight(double pixelCoords[2]);

	// Event handler for a mouse move event.
	void mouseMoveEvent(double pixelCoords[2], int buttonState);

	// Event handler for a mouse move event.
	void mouseScrollEvent(double pixelCoords[2], int yOffset);

	//---------------------------------------------------------------------------------------------------------------------
	//  Coordinate systems.
	//---------------------------------------------------------------------------------------------------------------------

	glm::vec4 pixelCoordsToWorldCoords(double pixelCoords[2]);

	//---------------------------------------------------------------------------------------------------------------------
	//  Window functions.
	//---------------------------------------------------------------------------------------------------------------------

	// Callback that handles the resizing event.
	void resizeEvent(int width, int height);

};

//-------------------------------------------------------------------------------------------------------------------------
//  EOF.
//-------------------------------------------------------------------------------------------------------------------------