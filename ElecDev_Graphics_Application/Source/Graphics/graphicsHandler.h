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

//----------------------------------------------------------------------------------------------------------------------
//  Structs.
//----------------------------------------------------------------------------------------------------------------------

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;

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

	// Variable that holds the active engine.
	std::string activeEngine;

	// Different drawing engines.
	DrawingEngineGL drawingEngine;
	DesignEngineGL designEngine;

	//-----------------------------------------------------------------------------------------------------------------
	//  Inits.
	//-----------------------------------------------------------------------------------------------------------------

	// Constructor.
	GraphicsHandler();

	// Graphics init.
	void initGraphics();

	//-----------------------------------------------------------------------------------------------------------------
	//  Engines.
	//-----------------------------------------------------------------------------------------------------------------

	// Function that handles which engine should be active.
	void renderGraphics();

	// Function that closes the engine passed.
	void closeEngine(std::string engine);

	//-----------------------------------------------------------------------------------------------------------------
	//  Shaders.
	//-----------------------------------------------------------------------------------------------------------------

	// Functions that handles shader creation from the shader files.
	static ShaderProgramSource parseShader(std::string& filePath);
	
};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------