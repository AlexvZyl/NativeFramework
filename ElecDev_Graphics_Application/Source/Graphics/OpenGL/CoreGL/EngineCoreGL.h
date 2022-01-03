#pragma once

/*
The graphics engines are drawn in ImGUI by creating a window for each instance of every different window.
To be able to keep the engines dynamic and add different types as required they all come from the same parent,
the EngineCoreGL class.  This is the pointer type that will be used to point to all of the child classes and their
members.  However, this will require that the core contains all of the different functions that will be called, as well
as error handling for when a wrong function is called on a engine type.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "ErrorHandlerGL.h"		// For OpenGL error handling.
#include <glm.hpp>				// OpenGL maths.
#include "GUI/GUIState.h"		// The GUI states and other information.
#include <string>

//=============================================================================================================================================//
//  Forward declerations.																													   //
//=============================================================================================================================================//

class Scene;

//=============================================================================================================================================//
//  Variables and constants.																												   //
//=============================================================================================================================================//

const float PI = 3.14159265358979323f;

//=============================================================================================================================================//
//  EngineCoreGL Class.																														   //
//=============================================================================================================================================//

class EngineCoreGL
{
public:

	// ---------- //
	//  S C E N E //
	// ---------- //
	
	std::unique_ptr<Scene> m_scene;

	// --------------------------- //
	//  S T A T E   M A C H I N E  //
	// --------------------------- //

	GUIState* m_guiState;										// Contains the current state of the application as well as information
																// that is required from the GUI side for the Graphics side.

	// ----------------- //
	//  V I E W P O R T  //
	// ----------------- //

	float m_imGuiViewportDimensions[2] = { 500, 500 };			// Stores the dimensions of the viewport that the OpenGL context gets drawn to.  

	// Resizes the viewport, projection matrix and FBO.
	virtual void resizeEvent(float width, float height);

	// ------------------------------------------------- //
	//  C O N S T R U C T O R   &   D E S T R U C T O R  //
	// ------------------------------------------------- //

	EngineCoreGL(GUIState* guiState, std::string contextName);
	~EngineCoreGL();

	// --------------------------------------- //
	//  R E N D E R I N G   F U N C T I O N S  //
	// --------------------------------------- //

	// Main loop where the rendering happens.
	virtual void renderLoop();
	// Updates the buffers on the GPU with the data stored CPU side.  Required 
	// to call to update the elements that are displayed to the screen.
	virtual void updateGPU();
	// Autocenters the current scene.
	virtual void autoCenter();
	// Draws elements to the screen.  Used for debugging and benchmarking.
	// Should also be implemented to showcase the capabilities of the engine.
	virtual void drawDemo(unsigned int loopCount);
	// Returns the FBO texture ID that can be rendered.
	virtual unsigned int getRenderTexture();
	// Creates the default background for the engines.
	void createDefaultBackground();
	// Calculates the delta time and returns it.	
	// This is used to ensure constant movement and sensitivity
	// regardless of the framerate.
	float deltaTime();
	// Returns the ID of the entity in the pixel coords.
	unsigned int getEntityID(float pixelCoords[2]);
	// Calculate the world coordinates from the pixel coordinates.
	virtual glm::vec3 pixelCoordsToWorldCoords(float pixelCoords[2]);
	// Calculate the camera coordinates from the pixel coordinates.
	virtual glm::vec3 pixelCoordsToCameraCoords(float pixelCoords[2]);

	float m_deltaTime = 0.0f;	// The difference between the last 2 rendred frame.
	float m_lastFrame = 0.0f;	// The time of the previous frame.

	std::string m_contextName = "";	// Name given to the context.

	// --------------------- //
	//  U S E R   I N P U T  //
	// --------------------- //

	float m_prevMouseEventPixelCoords[2] = { NULL, NULL };		// Stores the previous mouse event information.
	glm::vec3 m_prevMouseEventWorldVec = { NULL, NULL, NULL };	// Stores the vector that goes into the world.
	virtual void mousePressLeft(float pixelCoords[2]);
	virtual void mousePressRight(float pixelCoords[2]);
	virtual void mousePressMiddle(float pixelCoords[2]);
	virtual void mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle);
	virtual void mouseScrollEvent(float pixelCoords[2], float yOffset);
	virtual void keyEvent(int key, int action);

	// ------------------------------- //
	//  E R R O R   F U N C T I O N S  //
	// ------------------------------- //

	void functionNotImplementedError(std::string functionName);	// This is called when a funtion in EngineCoreGL is called but was not 
																// implemented in the child engine.
};

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//