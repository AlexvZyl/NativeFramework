/*  
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

//  General.
#include <string>

// File streaming.
#include <iostream>
#include <fstream>
#include <sstream>

// Class include.
#include "graphicsHandler.h"

// OpenGL
#include <glad/glad.h>
#include <ErrorHandler/errorHandler.h>

//----------------------------------------------------------------------------------------------------------------------
//  Inits.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
GraphicsHandler::GraphicsHandler()
{
	activeEngine = "Animation";
};

// Function that does the necessary inits for OpenGL and the engines.
void GraphicsHandler::initGraphics() 
{	
	// Compile shader.
	std::string shaderPath = "\\Shaders\\conanShader.shader";
	ShaderProgramSource conanShader = this->parseShader(shaderPath);

}

//----------------------------------------------------------------------------------------------------------------------
//  Main loop & engines handler.
//----------------------------------------------------------------------------------------------------------------------

// [MAIN DRAWING FUNCTION] Function that handles which engine should be active.
void GraphicsHandler::renderGraphics()
{

	//  Run engine that has been set as active.
	if (activeEngine == "DrawingEngine")
	{
		drawingEngine.test();
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
		std::cout << "ERROR: Please supply a valid engine name.\n";
	};

};

//----------------------------------------------------------------------------------------------------------------------
//  Shaders.
//----------------------------------------------------------------------------------------------------------------------

ShaderProgramSource GraphicsHandler::parseShader(std::string& filePath)
{	

	// Shader mode handler. Used as an index for the shaderStream.
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType shaderType = ShaderType::NONE;

	// Read shader from file.
	std::ifstream fileStream(filePath);
	// Stream that contains the shader.
	std::stringstream shaderStream[2];

	// Run through all of the lines.
	std::string line;
	while (std::getline(fileStream, line))
	{	
		// Find shader keyword and set shader type.
		if (line.find("#shader") != std::string::npos) 
		{	
			// Check for vertex shader.
			if (line.find("vertex") != std::string::npos)
				shaderType = ShaderType::VERTEX;
			// Check for fragment shader.
			else if (line.find("fragment") != std::string::npos)
				shaderType = ShaderType::FRAGMENT;
		}

		// Any other line of code.
		else
		{
			shaderStream[(int)shaderType] << line << "\n";
		}
	}

	return { shaderStream[0].str(), shaderStream[1].str() };

}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------