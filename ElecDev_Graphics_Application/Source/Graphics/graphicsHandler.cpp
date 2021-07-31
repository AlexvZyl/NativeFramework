/*  
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

//  General.
#include <string>
#include <iostream>

// Drawing engine.
#include "OpenGL/Engine2D/DrawingEngine/drawingEngine.h"
// Design engine.
#include "OpenGL/Engine2D/DesignEngine/designEngine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Graphics Handler Class.
//----------------------------------------------------------------------------------------------------------------------

class GraphicsHandler
{
public:

	//--------------------------------------------------------------------------------------------------------------
	//  Variables.
	//--------------------------------------------------------------------------------------------------------------

	std::string activeEngine;

	DrawingEngineGL drawingEngine;
	DesignEngineGL designEngine;

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor (Inits the class).
	GraphicsHandler()
	{

	}

	// Function that handles which engine should be active.
	void renderGraphics()
	{
		//  Run engine that has been set as active.
		if (activeEngine == "DrawingEngine")
		{
			// Run drawingEngine.
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
			std::cout << "ERROR: Please supply a valid engine name.\n";
		};
	};

	// Function that closes the engine passed.
	void closeEngine(std::string engine)
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
		else
		{
			std::cout << "ERROR: Please supply a valid engine name.\n";
		};
		
	}

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------