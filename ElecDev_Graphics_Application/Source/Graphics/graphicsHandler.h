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
	RenderWindowGL(GUIState* guiState, EngineType engineType)
	{
		if (engineType == EngineType::Base2DEngineGL)
		{
			engineGL = std::make_shared<Base2DEngineGL>(guiState);
			engineType = EngineType::Base2DEngineGL;
		}
		else if (engineType == EngineType::Design2DEngineGL)
		{
			engineGL = std::make_shared<Design2DEngineGL>(guiState);
			engineType = EngineType::Design2DEngineGL;
		}
		else if (engineType == EngineType::Base3DEngineGL)
		{
			engineGL = std::make_shared<Base3DEngineGL>(guiState);
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
struct InputEvent
{
	// Mouse press event information.
	bool mousePressEvent = false;;				// Was a mouse button pressed?
	//glm::vec2 mousePressPixels = { 0.f, 0.f };	// Pixel coords of mouse press.
	int mousePressButton = 0;					// The mouse button that was pressed.
	int mousePressAction = 0;					// Was it pressed or released?

	// Mouse move event.
	bool mouseMoveEvent = false;				// Did a mouse move event occur?
	//glm::vec2 mouseMovePixels = { 0.f, 0.f };	// Position of the mouse.
	int mouseMoveButtonStateLeft = 0;			// Left mouse pressed or released.
	int mouseMoveButtonStateRight = 0;			// Righ mouse pressed or released.
	int mouseMoveButtonStateMiddle = 0;			// Middle mouse pressed or released.

	// Mouse scroll event.
	bool mouseScrollEvent = false;				// Did a scroll event occur?
	float mouseScrollY = 0;						// Amount of scrolling.

	// Key event.
	bool keyEvent = false;						// Did a key event occur?
	int key = 0;								// The key that triggered the event.
	int keyAction = 0;							// Is the key pressed or released?
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
	InputEvent inputEvent;
	// The window.
	GLFWwindow* m_glfwWindow;

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

	// -------------------------- //
	//  W I N D O W   E V E N T S //
	// -------------------------- //

	// Resizes the engines.
	void resizeEvent(int width, int height);
	// Checks if the window name supplied is in the list.
	bool isWindowValid(std::shared_ptr<RenderWindowGL> renderWindow);
	// Checks if the window name supplied is in the list.
	bool isWindowValid(std::string windowName);

	bool isActiveWindowValid();

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