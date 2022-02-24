#pragma once

//  NOTE:  THIS FILE IS TO BE DEPRECATED!

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

#include "imgui/imgui.h"

/*=======================================================================================================================================*/
/* Forward declerations                                                                                                                  */
/*=======================================================================================================================================*/

class Component2D;
class Design2DEngine;
class Cable;

/*=======================================================================================================================================*/
/* Struture.                                                                                                                             */
/*=======================================================================================================================================*/

struct Context
{
	bool background = false;
	bool component = false;
	bool port = false;
	bool primative = false;
};

struct GUIState
{
	// Default constructor.
	GUIState() {}

	// Window States.
	bool toolsExpanded = false;
	bool drawMCC = false;
	bool blockDiagram = false;
	bool circuitBucket = false;
	bool startMainGraphics = true;
	bool startDemoWindow = false;
	bool showGraphicsWindow = true;
	bool circuitEditor = false;

	// Context menu states.
	Context clickedZone;

	// Popup menu.
	glm::vec2 popUpPosition = { 0.f, 0.f };
	bool popUpMenu = false;
	bool componentEditor = false;

	// Action states.
	bool renderWindowHovered = false;
	bool renderResizeEvent = true;
	bool globalQuit = false;
	bool userWindowHovered = false;

	// ImGui Variables.
	glm::vec2 imguiGlobalMouseCoords = { 0.f, 0.f };

	// Render windows information.
	glm::vec2 renderWindowMouseCoordinate = { 0,0 };
	glm::vec2 renderWindowSize = { 500,500 };

	// Engine.
	bool portHovered = false;
	bool placingComponent = false;

	//Design Engine
	Design2DEngine* design_engine = NULL;
	Component2D* active_component = NULL;
	Cable* active_cable = nullptr;

};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/