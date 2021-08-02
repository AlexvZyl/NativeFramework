/*  
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

//  General.
#include <string>

// Class include.
#include "graphicsHandler.h"

// OpenGL
#include <glad/glad.h>
#include <ErrorHandler/errorHandler.h>
#include "Shaders/shaderHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Inits.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
GraphicsHandler::GraphicsHandler()
{
	activeEngine = "DrawingEngine";
};

//----------------------------------------------------------------------------------------------------------------------
//  Main loop & engines handler.
//----------------------------------------------------------------------------------------------------------------------

// [MAIN DRAWING FUNCTION] Function that handles which engine should be active.
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
void GraphicsHandler::closeEngine(std::string engine)
{
	// Close the active engine.
	if (activeEngine == "DrawingEngine")
	{
		// Close.
	}
	else if (activeEngine == "DesignEngine")
	{
		// Close.
	}
	else if (activeEngine == "Animatin")
	{
		// Close animation.
	}
	else
	{
		std::cout << "[ENGINES ERROR] Please supply a valid engine name.\n";
	};

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------