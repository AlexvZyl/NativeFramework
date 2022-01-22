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
class ComponentEditor;
class PopUpMenu;
class CircuitEditor;

struct GLFWwindow;

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
	ImFont* m_fontRobotoMedium;
	bool dockCreated = false;
	ImGuiID dock_up_id;
	ImGuiID dock_right_id;
	ImGuiID dock_left_id;
	ImGuiID dock_down_id;
	ImGuiID dock_none_id;
	PyInterface* m_pyInterface;
	ImGuiID dock;
	ImGuiID dockSpaceID;

	// ----------------------------- //
	//  G U I   C O M P O N E N T S  //
	// ----------------------------- //

	std::unique_ptr<ComponentEditor> m_componentEditor;
	std::unique_ptr<PopUpMenu> m_popUpMenu;
	std::unique_ptr<Toolbar> m_toolbar;
	std::unique_ptr<Ribbons> m_ribbons;
	std::unique_ptr<userGUI> m_userGUIP;
	std::unique_ptr<GraphicsScene> m_graphicsScene;
	std::unique_ptr<CircuitEditor> m_circuitEditor;

	// ------------------- //
	//  F U N C T I O N S  //
	// ------------------- //

	// Constructor.
	GUIHandler(GUIState* guiState, GraphicsHandler* graphicsHandler, PyInterface* pyInterface);
	// Destructor.
	~GUIHandler();
	// Function that handles which engine should be active.
	void renderGui(GLFWwindow* window);
	// Set custom theme.
	void setTheme();
	void createDock(ImVec2 work_size);
	void resetDock(ImGuiID dockspace_id);
};

/*=======================================================================================================================================*/
/* EOF.																																     */
/*=======================================================================================================================================*/