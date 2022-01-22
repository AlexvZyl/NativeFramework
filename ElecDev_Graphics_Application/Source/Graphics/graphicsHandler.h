#pragma once

/*
This class will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//=============================================================================================================================================//
//  Includes.																																                                                                   //
//=============================================================================================================================================//

#include <string>
#include <variant>
#include <map>
#include "OpenGL/SceneGL.h"
#include "Engines/Core/EngineCore.h"
#include "Engines/Base2DEngine/Base2D_Engine.h"
#include "Engines/Base3DEngine/Base3D_Engine.h"
#include "Engines/Design2DEngine/Design2D_Engine.h"

//=============================================================================================================================================//
//  Forward declerations.			    											                                                           //
//=============================================================================================================================================//

struct GLFWwindow;

//=============================================================================================================================================//
//  Render Window data.																														                                                             //
//=============================================================================================================================================//

// Type that holds the different engines available.
enum class EngineType
{
	None = -1, Base2DEngine = 0, Design2DEngine = 1, Base3DEngine = 2
};

// Data type that holds the window that is to be drawn, as well as information regarding the window.
// Should be able to hold all of the different types of OpenGL engines.
struct RenderWindowGL
{
	// The rendering engine.
	// This is a pointer to the base engine.  With the use of virtual functions and dynamic casting
	// it will be able to point to subclasses as well.
	std::shared_ptr<EngineCore> engineGL;
	EngineType engineType = EngineType::None;

	// Data from ImGUI.
	float viewportDimentions[2];	//
	float mouseCoords[2];			//
	std::string windowName;			// The window name of the scene.

	// State machine information.
	bool isHovered = false;			// Is the mouse currently hovering over the window?
	bool isFocused = false;			// Is the window focused by ImGui?
	bool resizeEvent = true;		// Did a resize event occur for the window?
	bool close = true;				// Determines if the window should close.

	// Constructors.
	RenderWindowGL(){};
	RenderWindowGL(GUIState* guiState, EngineType engineType, std::string windowName)
		: windowName(windowName)
	{
		if (engineType == EngineType::Base2DEngine)
		{
			engineGL = std::make_shared<Base2DEngine>(guiState, windowName);
			engineType = EngineType::Base2DEngine;
		}
		else if (engineType == EngineType::Design2DEngine)
		{
			engineGL = std::make_shared<Design2DEngine>(guiState, windowName);
			engineType = EngineType::Design2DEngine;
		}
		else if (engineType == EngineType::Base3DEngine)
		{
			engineGL = std::make_shared<Base3DEngine>(guiState, windowName);
			engineType = EngineType::Base3DEngine;
		}
		viewportDimentions[0] = engineGL->m_scene->getViewport().x;
		viewportDimentions[1] = engineGL->m_scene->getViewport().y;
	}

	// Destructor.
	~RenderWindowGL(){}
};

// Event for handling saving scenes to disk.
struct SaveEvent
{
	bool eventTrigger = false;
	std::string path;
	std::string saveEngine;
};

// Event for handling saving scenes to disk.
struct LoadEvent
{
	bool eventTrigger = false;
	std::string path;

	void reset() 
	{
		eventTrigger = false;
		path = "";
	}
};

//=============================================================================================================================================//
//  Graphis Handler.																														                                                               //
//=============================================================================================================================================//

class GraphicsHandler
{
public:

	// ------------------- //
	//  V A R I A B L E S  //
	// ------------------- //

	// Map that holds the windows and their names.
	std::map<std::string, std::shared_ptr<RenderWindowGL>> m_windowsDictionary;
	// Variable that holds the active window name.
	std::shared_ptr<RenderWindowGL> m_activeWindow;
	// State machine variable.
	GUIState* m_guiState;
	// Stores information regarding a save event.
	SaveEvent m_saveEvent;
	// Stores information regarding a load event.
	LoadEvent m_loadEvent;
	// The window.
	GLFWwindow* m_glfwWindow;
	// Check if a window should be added.
	bool m_addWindow = false;
	std::string m_newWindowTitle = "";

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	// Constructor with GLFW window.
	GraphicsHandler(GUIState* guiState, GLFWwindow* glfwWindow);
	// Destructor.
	~GraphicsHandler();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Renders the graphics of all of the engines.
	// Can be improved by only rendering the engines that are viewable.  Does ImGUI do this automatically?
	void renderLoop();

	// --------------------- //
	//  U S E R   I N P U T  //
	// --------------------- //

	// Mouse events are automatically assigned to the active window.
	// The active window is changed from the ImGUI side as the mouse moves.
	void mousePressEvent();		// Handle mouse press events.
	void mouseMoveEvent();		// Handle mouse move events.
	void mouseScrollEvent();	// Handle mouse scroll events.
	void keyEvent();			// Handle keyboard inputs.

	// ------------ //
	//  E V E N T S //
	// ------------ //

	// Resizes the engines.
	void resizeEvent();
	// Checks if the window name supplied is in the list.
	bool isWindowValid(std::shared_ptr<RenderWindowGL> renderWindow);
	// Checks if the window name supplied is in the list.
	bool isWindowValid(std::string windowName);
	// Checks if the currently active window is valid.
	bool isActiveWindowValid();
	// Handles file drop events.
	void fileDropEventHandler();
	// Handles save events.
	void saveEventHandler();
	// Handles load events.
	void loadEventHandler();

	// ------------- //
	//  2 D   A P I  //
	// ------------- //

	// Functions supported by the base engine.
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

	// ------------- //
	//  3 D   A P I  //
	// ------------- //

	void drawQuadFilled3D(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float color[4]);
	void drawCuboidFilled(std::string windowName, float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float depth, float color[4]);

	// ----------------------------------- //
	//  W I N D O W   M A N A G E M E N T  //
	// ----------------------------------- //

	// Adds an window with an OpenGL context.
	void addWindow(std::string windowName, std::string engineType);
	// Removes the window from ImGui.
	void removeWindow(std::string windowName);
	void windowError(std::string windowName);
	void parametersError(const std::exception& e);

	// -------------- //
	//  T E S T I N G //
	// -------------- //

	// Draws a demo.
	void drawDemo(std::string windowName, unsigned int loopCount);
};

//=============================================================================================================================================//
//  EOF.																																	                                                                     //
//=============================================================================================================================================//