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
#include "OpenGL/Engine2D/DesignEngine/designEngineCore.h"
// Drawing engine.
#include "OpenGL/Engine2D/BaseEngine/baseEngineCore.h"

//  General.
#include <string>

// OpenGL
#include <glad/glad.h>
#include <ErrorHandler/errorHandler.h>
#include "ShaderHandler/shaderHandler.h"
#include "../stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Data structures
//----------------------------------------------------------------------------------------------------------------------

// Struct that contains the different OpenGL engines that can be used.
enum class Engines
{
	ANIMATION, BASE_ENGINE, DESIGN_ENGINE
};

//----------------------------------------------------------------------------------------------------------------------
//  Graphics Handler Class.
//----------------------------------------------------------------------------------------------------------------------

class GraphicsHandler
{
public:

	//-----------------------------------------------------------------------------------------------------------------
	//  Variables.
	//-----------------------------------------------------------------------------------------------------------------

	// State machine variable.
	stateMachine* m_states;

	// Variable that holds the active engine.
	Engines m_activeEngine;

	// Different drawing engines.
	BaseEngineGL* m_drawingEngine;
	DesignEngineGL* m_designEngine;

	// The GLFW window from the main application.
	GLFWwindow* m_window;

	//-----------------------------------------------------------------------------------------------------------------
	//  Constructors and setup.
	//-----------------------------------------------------------------------------------------------------------------

	// Constructor with GLFW window.
	GraphicsHandler(GLFWwindow* window, stateMachine* states);
	// Destructor.
	~GraphicsHandler();

	//-----------------------------------------------------------------------------------------------------------------
	//  Functions.
	//-----------------------------------------------------------------------------------------------------------------

	// Function that handles which engine should be active.
	void renderGraphics();

	// Function that sets the new active engine.
	void setEngine(std::string engine);

	// Function that closes the engine passed.
	void closeActiveEngine();

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