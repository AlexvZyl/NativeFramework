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

class Graphics
{
public:

	//Variables
	stateMachine* states;
	ImVec2 pos;
	GraphicsHandler* graphicsHandler;

	//Constructor
	Graphics(stateMachine* states, GraphicsHandler* graphicsHandler);

	//Renderer
	void renderGraphics();
};

