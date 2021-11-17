#pragma once

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

#include "imgui.h"

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
	GUIState(){}

	// Window States.
	bool toolsExpanded = false;
	bool drawMCC = false;
	bool blockDiagram = false;
	bool circuitBucket = false;
	bool startMainGraphics = true;
	bool startDemoWindow = false;
	bool showGraphicsWindow = true;

	// Context menu states.
	Context clickedZone;

	// Popup menu.
	ImVec2 popUpPosition = { 0.f, 0.f };
	bool popUpMenu = false;
	bool componentEditor = false;

	// Action states.
	bool renderWindowHovered = false;
	bool renderResizeEvent = true;
	bool globalQuit = false;
	bool userWindowHovered = false;

	// ImGui Variables.
	ImVec2 imguiGlobalMouseCoords = {0.f, 0.f};

	// Render windows information.
	ImVec2 renderWindowMouseCoordinate = { 0,0 };
	ImVec2 renderWindowSize = { 500,500 };

	// Engine.
	bool portHovered = false;
	bool placingComponent = false;

};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/