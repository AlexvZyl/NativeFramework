#pragma once

/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include <Core/imgui.h>
#include <string>
#include "../stateMachine.h"
#include <../Graphics/graphicsHandler.h>
#include <GLFW/glfw3.h>

/*=======================================================================================================================================*/
/* Class.	                                                                                                                             */
/*=======================================================================================================================================*/

class MCC
{
public:

	//Variables
	stateMachine* states;
	ImVec2 pos;
	std::map<std::string, BaseEngineGL> *MCCDict;
	ImGuiID dock;

	//Constructor
	MCC(stateMachine* states, std::map<std::string, BaseEngineGL> *MCCDict);

	//Renderer
	void renderGraphics(ImGuiID dock);
	void deQueueInput();
};

