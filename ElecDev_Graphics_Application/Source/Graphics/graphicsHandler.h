#pragma once

/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Design engine.
#include "OpenGL/Engine2D/DesignEngine/designEngine.h"
// Drawing engine.
#include "OpenGL/Engine2D/DrawingEngine/drawingEngine.h"

//  General.
#include <string>

// OpenGL
#include <glad/glad.h>
#include <ErrorHandler/errorHandler.h>
#include "Shaders/shaderHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Graphics Handler Class.
//----------------------------------------------------------------------------------------------------------------------

class GraphicsHandler
{
public:

	//-----------------------------------------------------------------------------------------------------------------
	//  Variables.
	//-----------------------------------------------------------------------------------------------------------------

	// Variable that holds the active engine.
	std::string activeEngine;

	// Different drawing engines.
	DrawingEngineGL drawingEngine;
	DesignEngineGL designEngine;

	// The GLFW window from the main application.
	GLFWwindow* window;

	//-----------------------------------------------------------------------------------------------------------------
	//  Constructors and setup.
	//-----------------------------------------------------------------------------------------------------------------

	// Constructor with GLFW window.
	GraphicsHandler(GLFWwindow* window);
	// Default constructor.
	GraphicsHandler();

	//-----------------------------------------------------------------------------------------------------------------
	//  Functions.
	//-----------------------------------------------------------------------------------------------------------------

	// Function that handles which engine should be active.
	void renderGraphics();

	// Function that sets the new active engine.
	void setEngine(std::string engine);

	// Function that closes the engine passed.
	void closeEngine();

	//-----------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//-----------------------------------------------------------------------------------------------------------------

	// Handle mouse press events.
	void mousePressEvent(GLFWwindow* window, int button, int action, int mods);

	// Handle mouse move events.
	void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos);

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------