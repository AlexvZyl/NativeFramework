#pragma once

/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "Core/imgui.h"

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
#include "../Helper/stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Graphics Handler Class.
//----------------------------------------------------------------------------------------------------------------------

class GraphicsHandler
{
public:

	//-----------------------------------------------------------------------------------------------------------------
	//  Variables.
	//-----------------------------------------------------------------------------------------------------------------

	stateMachineGraphics states;

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
	GraphicsHandler(GLFWwindow* window, stateMachineGraphics* states);
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

	// Handle mouse move events.
	void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset);

	//-----------------------------------------------------------------------------------------------------------------
	//  Window events.
	//-----------------------------------------------------------------------------------------------------------------

	void resizeEvent(GLFWwindow* window, int width, int height);

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------