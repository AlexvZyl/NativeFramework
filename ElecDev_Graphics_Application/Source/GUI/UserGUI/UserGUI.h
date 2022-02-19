#pragma once
#pragma once

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include <imgui/imgui.h>
#include <string>
#include <list>

/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

class PyInterface;
class GraphicsHandler;

/*=======================================================================================================================================*/
/* Strutures. 																															 */
/*=======================================================================================================================================*/

struct element
{
	std::string name;
	std::list<std::string> extraParams;
	std::string type;
	std::string data = "";

	element(std::string typeIn, std::string nameIn, std::list<std::string> extra) {
		name = nameIn;
		extraParams = extra;
		type = typeIn;
	}
};

struct guiHolder
{
	std::string windowName;
	ImVec2 windowPos;
	std::list<element> elements;
	bool close = true;
	bool submit = false;
	bool docked = false;
	bool addDock = false;
	std::string docking;
	ImGuiID dockPos;

	bool first = true;

	guiHolder(std::string window, ImVec2 windowPosIn, std::string dockingIn, std::list<element> inElements)
	{
		windowName = window;
		elements = inElements;
		windowPos = windowPosIn;
		docking = dockingIn;
	}
};

/*=======================================================================================================================================*/
/* Class.																																 */
/*=======================================================================================================================================*/

class userGUI
{

public:

	//Variables
	PyInterface* m_pyInterface;
	ImVec2 pos;
	ImGuiID dock;
	bool userPrevious;
	bool close = true;
	const char* voltageLevel = "460";
	const char* efficiencyRating = "IEC";
	char steadyVoltDrop[128] = "Hello, world!";
	std::list<guiHolder> guis;
	bool resetDock = false;

	//Constructor
	userGUI(PyInterface* pyInterface);

	//Renderer
	void pushData(std::list<guiHolder>::iterator uiWindow);
	void createGUI(std::string guiName, std::string guiPos, std::string docking, std::string parameters);
	void renderUI(ImGuiID* dock);
};

/*=======================================================================================================================================*/
/* EOF.	     																															 */
/*=======================================================================================================================================*/