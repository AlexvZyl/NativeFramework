#pragma once

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

#include "imgui.h"

/*=======================================================================================================================================*/
/* Struture.                                                                                                                             */
/*=======================================================================================================================================*/

struct GUIState
{
	// Default constructor.
	GUIState() 
	{
	}

	// Window States.
	bool toolsExpanded = false;
	bool drawMCC = false;
	bool blockDiagram = false;
	bool circuitBucket = false;
	bool startMainGraphics = true;
	bool startDemoWindow = false;
	bool showGraphicsWindow = true;

	// Action states.
	bool renderWindowHovered=false;
	bool renderResizeEvent=true;
	bool globalQuit = false;
	bool userWindowHovered = false;

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