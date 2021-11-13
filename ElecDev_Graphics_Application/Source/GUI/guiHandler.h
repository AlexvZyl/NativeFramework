#pragma once

/*=======================================================================================================================================*/
/* Includes.                                                                                                                             */
/*=======================================================================================================================================*/

//  General.
#include <string>
#include <iostream>
#include <map>
#include <list>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>
#include "GUIState.h"
#include "GLFW/glfw3.h"

/*=======================================================================================================================================*/
/* Forward Declerations.                                                                                                                 */
/*=======================================================================================================================================*/

// App components.
class GraphicsHandler;
class PyInterface;
// GUI components.
class Ribbons;
class Toolbar;
class userGUI;
class GraphicsScene;

/*=======================================================================================================================================*/
/* Class.																																 */
/*=======================================================================================================================================*/

class GUIHandler
{
public:

	// ------------------ //
	//  V A R I A B L E S //
	// ------------------ //

	GUIState* m_guiState;

	GraphicsHandler* graphicsHandler;

	unsigned int textureID;

	Toolbar* toolbar;
	Ribbons* ribbons;
	userGUI* userGUIP;
	GraphicsScene* m_graphicsScene;
	ImGuiID dock;
	ImGuiID dockSpaceID;

	bool dockCreated = false;

	ImGuiID dock_up_id;
	ImGuiID dock_right_id;
	ImGuiID dock_left_id;
	ImGuiID dock_down_id;
	ImGuiID dock_none_id;

	PyInterface* m_pyInterface;

	// ------------------- //
	//  F U N C T I O N S  //
	// ------------------- //

	// Constructor.
	GUIHandler(GUIState* guiState, GraphicsHandler* graphicsHandler, PyInterface* pyInterface);
	// Destructor.
	~GUIHandler();

	// Function that handles which engine should be active.
	void renderGui(ImGuiIO& io, GLFWwindow* window);

	// Set custom theme.
	void setTheme();

	void createDock(ImVec2 work_size);
	void resetDock(ImGuiID dockspace_id);
};

/*=======================================================================================================================================*/
/* EOF.																																     */
/*=======================================================================================================================================*/