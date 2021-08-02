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

//----------------------------------------------------------------------------------------------------------------------
//  The drawingEngine class.
//----------------------------------------------------------------------------------------------------------------------

class DrawingEngineGL
{
public:

	//---------------------------------------------------------------------------------------------------------------------
	// MVP Matrices.
	//---------------------------------------------------------------------------------------------------------------------

	

	//---------------------------------------------------------------------------------------------------------------------
	//  Variables.
	//---------------------------------------------------------------------------------------------------------------------

	Shader basicShader;
	unsigned int VAO;

	//---------------------------------------------------------------------------------------------------------------------
	//  Constructor.
	//---------------------------------------------------------------------------------------------------------------------

	DrawingEngineGL();

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

};

//-------------------------------------------------------------------------------------------------------------------------
//  EOF.
//-------------------------------------------------------------------------------------------------------------------------