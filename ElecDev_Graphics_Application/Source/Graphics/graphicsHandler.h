#pragma once

/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <ErrorHandler/ErrorHandler.h>
#include "ShaderHandler/ShaderHandler.h"
#include "../stateMachine.h"

// Drawing engine.
#include "OpenGL/Engines/Base2DEngine/Base2D_Engine.h"
// Design engine.
#include "OpenGL/Engines/Design2DEngine/Design2D_Engine.h"
// 3D Engine.
#include "OpenGL/Engines/Base3DEngine/Base3D_Engine.h"

//  General.
#include <string>
#include <variant>

// GUI.
#include "Core/imgui.h"

// OpenGL
#include <glad/glad.h>

//----------------------------------------------------------------------------------------------------------------------
//  Data
//----------------------------------------------------------------------------------------------------------------------

// Type that holds the different engines available.
enum class EngineType
{
	None = -1, BaseEngineGL = 0, DesignEngineGL = 1, Base3DEngineGL = 2
};

// Data type that holds the window that is to be drawn, as well as information regarding the window.
// Should be able to hold all of the different types of OpenGL engines.
struct RenderWindowGL 
{
	// The rendering engine.
	// This is a pointer to the base engine.  With the use of virtual functions and dynamic casting
	// it will be able to point to subclasses as well.
	BaseEngineGL* engineGL;
	EngineType engineType = EngineType::None;

	// Data from ImGUI.
	ImVec2 viewportDimentions;
	ImVec2 mouseCoords;
	std::string windowName;

	// State machine information.
	bool isHovered = false;
	bool resizeEvent = true;
	bool close = true;

	// Constructor.
	RenderWindowGL(stateMachine* states, EngineType engineType) 
	{
		if (engineType == EngineType::BaseEngineGL) 
		{
			engineGL = new BaseEngineGL(states);
			engineType = EngineType::BaseEngineGL;
		}
		else if (engineType == EngineType::DesignEngineGL)
		{
			engineGL =  new DesignEngineGL(states);
			engineType = EngineType::DesignEngineGL;
		}
		else if (engineType == EngineType::Base3DEngineGL)
		{
			engineGL = new Base3DEngineGL(states);
			engineType = EngineType::Base3DEngineGL;
		}
		viewportDimentions[0] = engineGL->m_imGuiViewportDimensions[0];
		viewportDimentions[1] = engineGL->m_imGuiViewportDimensions[1];
	}

	// Destructor.
	~RenderWindowGL()
	{
		delete engineGL;
	}
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

	// Map that holds the windows and their names.
	std::map<std::string, RenderWindowGL*> m_windowsDictionary;

	// Variable that holds the active window name.
	std::string m_activeWindow;

	// State machine variable.
	stateMachine* m_states;

	//-----------------------------------------------------------------------------------------------------------------
	//  Constructors and setup.
	//-----------------------------------------------------------------------------------------------------------------

	// Constructor with GLFW window.
	GraphicsHandler(stateMachine* states);
	// Destructor.
	~GraphicsHandler();

	//-----------------------------------------------------------------------------------------------------------------
	//  Functions.
	//-----------------------------------------------------------------------------------------------------------------

	// Renders the graphics of all of the engines.
	// Can be improved by only rendering the engines that are viewable.  Does ImGUI do this automatically?
	void renderGraphics();

	//-----------------------------------------------------------------------------------------------------------------
	//  Mouse events.
	//-----------------------------------------------------------------------------------------------------------------
	// Mouse events are automatically assigned to the active window.
	// The active window is changed from the ImGUI side as the mouse moves.

	void mousePressEvent(int button, int action);					// Handle mouse press events.
	void mouseMoveEvent(int buttonStateLeft, int buttonStateRight);	// Handle mouse move events.
	void mouseScrollEvent(float yOffset);							// Handle mouse scroll events.

	//-----------------------------------------------------------------------------------------------------------------
	//  Window events.
	//-----------------------------------------------------------------------------------------------------------------

	// Resizes the engines.
	void resizeEvent(int width, int height);
	// Checks if the window name supplied is in the list.
	bool isWindowValid(std::string windowName);

	//---------------------------------------------------------------------------------------
	// API.
	//---------------------------------------------------------------------------------------

	// 2D Rendering
	// --------------------------------
	
	// Functions supported by the base engine.  Added functionality to choose MCC.
	void drawLine(std::string windowName, float position1[2], float position2[2], float color[4]);
	void drawTriangleClear(std::string windowName, float position1[2], float position2[2], float position3[2], float color[4]);
	void drawTriangleFilled(std::string windowName, float position1[2], float position2[2], float position3[2], float color[4]);
	void drawQuadClear(std::string windowName, float position[2], float width, float height, float color[4]);
	void drawQuadFilled(std::string windowName, float position[2], float width, float height, float color[4]);
	void drawCircleClear(std::string windowName, float position[2], float radius, float color[4]);
	void drawCircleFilled(std::string windowName, float position[2], float radius, float color[4]);
	void drawText(std::string windowName, std::string text, float coords[2], float color[4], float scale, std::string align);
	// Scales and centers the drawing in the window.
	void autoCenter(std::string windowName);
	// Loads the CPU data to the GPU.
	void updateBuffers(std::string windowName);

	// 3D Rendering
	// --------------------------------

	void drawQuadFilled3D(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float color[4]);

	// Window management.
	// --------------------------------
	
	// Adds an MCC to the dict.
	void addWindow(std::string windowName, std::string engineType);
	// Removes an MCC from the dict.
	void removeWindow(std::string windowName);
	void windowError(std::string windowName);
	void parametersError(const std::exception &e);

	// Testing.
	// --------------------------------
	
	// Draws a demo.
	void drawDemo(std::string windowName, unsigned int loopCount);
};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------