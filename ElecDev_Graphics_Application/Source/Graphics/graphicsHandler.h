#pragma once

/*
This class will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

//  General.
#include <string>
#include <variant>
// OpenGL engines.
#include "OpenGL/CoreGL/EngineCoreGL.h"
#include "OpenGL/Base2DEngineGL/Base2D_Engine.h"
#include "OpenGL/Base3DEngineGL/Base3D_Engine.h"
#include "OpenGL/Design2DEngineGL/Design2D_Engine.h"

//=============================================================================================================================================//
//  Data.																																	   //
//=============================================================================================================================================//

// Type that holds the different engines available.
enum class EngineType
{
	None = -1, Base2DEngineGL = 0, Design2DEngineGL = 1, Base3DEngineGL = 2
};

// Data type that holds the window that is to be drawn, as well as information regarding the window.
// Should be able to hold all of the different types of OpenGL engines.
struct RenderWindowGL 
{
	// The rendering engine.
	// This is a pointer to the base engine.  With the use of virtual functions and dynamic casting
	// it will be able to point to subclasses as well.
	EngineCoreGL* engineGL;
	EngineType engineType = EngineType::None;

	// Data from ImGUI.
	float viewportDimentions[2];
	float mouseCoords[2];
	std::string windowName;

	// State machine information.
	bool isHovered = false;
	bool resizeEvent = true;
	bool close = true;

	// Constructor.
	RenderWindowGL(GUIState* guiState, EngineType engineType) 
	{
		if (engineType == EngineType::Base2DEngineGL) 
		{
			engineGL = new Base2DEngineGL(guiState);
			engineType = EngineType::Base2DEngineGL;
		}
		else if (engineType == EngineType::Design2DEngineGL)
		{
			engineGL =  new Design2DEngineGL(guiState);
			engineType = EngineType::Design2DEngineGL;
		}
		else if (engineType == EngineType::Base3DEngineGL)
		{
			engineGL = new Base3DEngineGL(guiState);
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

//=============================================================================================================================================//
//  Graphis Handler.																														   //
//=============================================================================================================================================//

class GraphicsHandler
{
public:

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //

	// Map that holds the windows and their names.
	std::map<std::string, RenderWindowGL*> m_windowsDictionary;

	// Variable that holds the active window name.
	std::string m_activeWindow;

	// State machine variable.
	GUIState* m_guiState;

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor with GLFW window.
	GraphicsHandler(GUIState* guiState);
	// Destructor.
	~GraphicsHandler();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Renders the graphics of all of the engines.
	// Can be improved by only rendering the engines that are viewable.  Does ImGUI do this automatically?
	void renderGraphics();

	// ------------------------- //
	//  M O U S E   E V E N T S  //
	// ------------------------- //
	
	// Mouse events are automatically assigned to the active window.
	// The active window is changed from the ImGUI side as the mouse moves.
	void mousePressEvent(int button, int action);					// Handle mouse press events.
	void mouseMoveEvent(int buttonStateLeft, int buttonStateRight);	// Handle mouse move events.
	void mouseScrollEvent(float yOffset);							// Handle mouse scroll events.

	// -------------------------- //
	//  W I N D O W   E V E N T S //
	// -------------------------- //

	// Resizes the engines.
	void resizeEvent(int width, int height);
	// Checks if the window name supplied is in the list.
	bool isWindowValid(std::string windowName);

	// ------------- //
	//  2 D   A P I  //
	// ------------- //
	
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

	// ------------- //
	//  3 D   A P I  //
	// ------------- //

	void drawQuadFilled3D(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float color[4]);
	void drawCuboidFilled(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float depth, float color[4]);

	// ----------------------------------- //
	//  W I N D O W   M A N A G E M E N T  //
	// ----------------------------------- //

	// Adds an MCC to the dict.
	void addWindow(std::string windowName, std::string engineType);
	// Removes an MCC from the dict.
	void removeWindow(std::string windowName);
	void windowError(std::string windowName);
	void parametersError(const std::exception &e);

	// -------------- //
	//  T E S T I N G //
	// -------------- //

	// Draws a demo.
	void drawDemo(std::string windowName, unsigned int loopCount);
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//