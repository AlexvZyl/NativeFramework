#pragma once

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
	int toolsMode = 0; // 0 = nothing, 1 = Draw MCC, 2 = Block Diagram, 3 = Draw Circuit Bucket.
	bool startMainGraphics = true;
	bool startDemoWindow = false;
	bool showGraphicsWindow = true;

	// Action states.
	bool renderWindowHovered=false;
	bool renderResizeEvent=true;
	bool globalQuit = false;
	bool userWindowHovered = false;

	// Render windows information.
	float renderWindowMouseCoordinate[2] = { 0,0 };
	float renderWindowSize[2] = { 500,500 };
};

/*=======================================================================================================================================*/
/* EOF.                                                                                                                                  */
/*=======================================================================================================================================*/