#pragma once

/*
This class will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will containt both ImGUI calls and pure OpenGL calls can remain clean.
*/

//=============================================================================================================================================//
//  Includes.																																                                                                   //
//=============================================================================================================================================//

//  General.
#include <string>
#include <variant>
#include <map>
// OpenGL engines.
#include "OpenGL/CoreGL/EngineCoreGL.h"
#include "OpenGL/Base2DEngineGL/Base2D_Engine.h"
#include "OpenGL/Base3DEngineGL/Base3D_Engine.h"
#include "OpenGL/Design2DEngineGL/Design2D_Engine.h"

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
	None = -1, Base2DEngineGL = 0, Design2DEngineGL = 1, Base3DEngineGL = 2
};

// Data type that holds the window that is to be drawn, as well as information regarding the window.
// Should be able to hold all of the different types of OpenGL engines.
struct RenderWindowGL
{
	// The rendering engine.
	// This is a pointer to the base engine.  With the use of virtual functions and dynamic casting
	// it will be able to point to subclasses as well.
	std::shared_ptr<EngineCoreGL> engineGL;
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
		if (engineType == EngineType::Base2DEngineGL)
		{
			engineGL = std::make_shared<Base2DEngineGL>(guiState, windowName);
			engineType = EngineType::Base2DEngineGL;
		}
		else if (engineType == EngineType::Design2DEngineGL)
		{
			engineGL = std::make_shared<Design2DEngineGL>(guiState, windowName);
			engineType = EngineType::Design2DEngineGL;
		}
		else if (engineType == EngineType::Base3DEngineGL)
		{
			engineGL = std::make_shared<Base3DEngineGL>(guiState, windowName);
			engineType = EngineType::Base3DEngineGL;
		}
		viewportDimentions[0] = engineGL->m_imGuiViewportDimensions[0];
		viewportDimentions[1] = engineGL->m_imGuiViewportDimensions[1];
	}

	// Destructor.
	~RenderWindowGL(){}
};

//=============================================================================================================================================//
//  Input event data.																														                                                               //
//=============================================================================================================================================//

// This struct is used to store the information on input events so that it can be handled on a
// frame by frame basis and not every single time it occurs.
struct m_inputEvent
{
	// Mouse press events.
	bool mousePressEvent			= false;		// Sets upon any press event.
	bool mousePressLeftEvent		= false;		// Mouse press left.
	bool mousePressRightEvent		= false;		// Mouse press right.
	bool mousePressMiddleEvent		= false;		// Mouse press middle.
	// Mouse release events.
	bool mouseReleaseEvent			= false;		// Sets upon any release event.
	bool mouseReleaseLeftEvent		= false;		// Mouse release left.
	bool mouseReleaseRightEvent		= false;		// Mouse release right.
	bool mouseReleaseMiddleEvent	= false;		// Mouse release middle.

	// Mouse move event.
	bool mouseMoveEvent				= false;		// Did a mouse move event occur?
	int mouseMoveButtonStateLeft	= 0;			// Left mouse pressed or released.
	int mouseMoveButtonStateRight	= 0;			// Righ mouse pressed or released.
	int mouseMoveButtonStateMiddle	= 0;			// Middle mouse pressed or released.

	// Mouse scroll event.
	bool mouseScrollEvent			= false;		// Did a scroll event occur?
	float mouseScrollY				= 0;			// Amount of scrolling.

	// Key event.
	bool keyEvent					= false;		// Did a key event occur?
	int key							= 0;			// The key that triggered the event.
	int keyAction					= 0;			// Is the key pressed or released?

	// Reset the mouse press data.
	void resetMousePress() 
	{
		mousePressEvent		  = false;
		mousePressLeftEvent   = false;
		mousePressRightEvent  = false;
		mousePressMiddleEvent = false;
	}

	// Reset the mouse release data.
	void resetMouseRelease()
	{
		mouseReleaseEvent		= false;
		mouseReleaseLeftEvent	= false;
		mouseReleaseRightEvent	= false;
		mouseReleaseMiddleEvent = false;
	}
};

// Contains the information for when files are dopped into the app.
struct FileDropEvent
{
	bool eventTrigger = false;			// Triggers with event.
	int totalFiles = 0;					// Amount of files dropped.
	std::vector<std::string> paths;		// Path to the files.
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
	// Stores the information on input events.
	m_inputEvent m_inputEvent;
	// Stores information regarding a save event.
	SaveEvent m_saveEvent;
	// Stores information regarding a load event.
	LoadEvent m_loadEvent;
	// The window.
	GLFWwindow* m_glfwWindow;
	// Stores information regarding the file drop event.
	FileDropEvent m_fileDropEvent;
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