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
	GraphicsHandler* graphicsHandler;
	ImVec2 pos;
	ImGuiID dock;

	//Constructor
	MCC(stateMachine* states, GraphicsHandler* graphicsHandler);

	//Renderer
	void renderGraphics(ImGuiID dock);
	void deQueueInput();
};

