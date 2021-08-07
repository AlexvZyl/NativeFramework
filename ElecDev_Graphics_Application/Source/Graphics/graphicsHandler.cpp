/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "graphicsHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructors.
//----------------------------------------------------------------------------------------------------------------------

// Default.
GraphicsHandler::GraphicsHandler() {};

// With GLFW window.
GraphicsHandler::GraphicsHandler(GLFWwindow* window)
{
	// Create engines.
	DrawingEngineGL drawingEngine(this->window);
	this->drawingEngine = drawingEngine;
	//DesignEngineGL designEngine(this->window);
	//this->designEngine = designEngine;

	// Set the default active engine.  (Should be set to animation when one is available.)
	activeEngine = "DrawingEngine";

	// Store pointer to GLFW window.
	this->window = window;

	// Setup mouse button callback.
	//glfwSetMouseButtonCallback(window, this->mousePressEvent);
};

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// [LOOP FUNCTION] Function that handles which engine should be active and is placed into the OpenGL loop.
void GraphicsHandler::renderGraphics()
{

	//  Run engine that has been set as active.
	if (activeEngine == "DrawingEngine")
	{
		drawingEngine.renderLoop();
	}
	else if (activeEngine == "DesignEngine")
	{
		// Run designEngine.
	}
	else if (activeEngine == "Animation")
	{
		// Run some sort of cool place holder animation.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Please supply a valid engine name.\n";
	};

};

// Function that closes the engine passed.
void GraphicsHandler::setEngine(std::string engine)
{
	// Close the current active engine.
	this->closeEngine();

	// Set the new active engine.
	if (activeEngine == "DrawingEngine")
	{
		// Init.
	}
	else if (activeEngine == "DesignEngine")
	{
		// Init.
	}
	else if (activeEngine == "Animatin")
	{
		// Init.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Please supply a valid engine name.\n";
	};
};

// Function that closes the engine passed.
void GraphicsHandler::closeEngine()
{
	// Close the active engine.
	if (this->activeEngine == "DrawingEngine")
	{
		// Close.
	}
	else if (this->activeEngine == "DesignEngine")
	{
		// Close.
	}
	else if (this->activeEngine == "Animation")
	{
		// Close animation.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Error occured closing the active engine.\n";
	};
};

//----------------------------------------------------------------------------------------------------------------------
//  Mouse events.
//----------------------------------------------------------------------------------------------------------------------

void GraphicsHandler::mousePressEvent(GLFWwindow* window, int button, int action, int mods) 
{
	// Left mouse button press.
	if ((button==GLFW_MOUSE_BUTTON_LEFT) && (action==GLFW_PRESS))
	{
		this->drawingEngine.mousePressLeft();
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------