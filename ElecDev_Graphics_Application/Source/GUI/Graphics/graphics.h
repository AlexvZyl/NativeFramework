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
	stateMachineGraphics* states;
	unsigned int textureID;
	GraphicsHandler* graphicsHandler;

	//Constructor
	Graphics(stateMachineGraphics* states, GraphicsHandler* graphicsHandler);

	//Renderer
	void renderGraphics();
};

