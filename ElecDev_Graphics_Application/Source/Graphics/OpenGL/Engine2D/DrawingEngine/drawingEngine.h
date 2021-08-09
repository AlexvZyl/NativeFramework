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
	glm::mat4 modelMatrix = glm::mat4(1.0f);		// The model matrix that places the object in the world.  Is going to be 
													// kept an identity matrix for now.
	glm::mat4 viewMatrix = glm::mat4(1.0f);			// The matrix that handles the camera movement.
													// viewMatrix = translatinMatrix * rotationMatrix * scalingMatrix;
	glm::mat4 projectionMatrix = glm::mat4(1.0f);	// The matrix that handles the clipping plane (which part of the world is
													// going to be visible to the screen?
	glm::mat4 viewportMatrix = glm::mat4(1.0f);		// The matrix that handles the viewport transform.  Converts screen pixel
													// coordinates to the OpenGL uniform coordinate system.

	// View matrix components.
	glm::mat4 sccalingMatrix = glm::mat4(1.0f);		// Handles camera scaling.
	glm::mat4 translationMatrix = glm::mat4(1.0f);	// Handles camera translations.
	glm::mat4 rotationMatrix = glm::mat4(1.0f);		// Handles camera rotations.

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
	//  Buffers.
	//---------------------------------------------------------------------------------------------------------------------
	
	unsigned int VAO;

	//---------------------------------------------------------------------------------------------------------------------
	//  Misc variables.
	//---------------------------------------------------------------------------------------------------------------------

	GLFWwindow* window;

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

	// Event handler for a mouse left press.
	void mousePressLeft(double pixelCoords[2]);

	// Event handler for a mouse move event.
	void mouseMoveEvent(double pixelCoords[2], int buttonState);

	//---------------------------------------------------------------------------------------------------------------------
	//  Coordinate systems.
	//---------------------------------------------------------------------------------------------------------------------

	glm::vec4 pixelCoordsToWorldCoords(double pixelCoords[2]);

};

//-------------------------------------------------------------------------------------------------------------------------
//  EOF.
//-------------------------------------------------------------------------------------------------------------------------