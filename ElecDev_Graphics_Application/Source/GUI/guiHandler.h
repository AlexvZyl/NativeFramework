#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <../Graphics/graphicsHandler.h>

//  General.
#include <string>
#include <iostream>

#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "Graphics/graphics.h"
#include "../stateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
//  GUI Handler Class.
//----------------------------------------------------------------------------------------------------------------------

class GUIHandler
{
public:

	//--------------------------------------------------------------------------------------------------------------
	//  Variables.
	//--------------------------------------------------------------------------------------------------------------

	stateMachine states;

	stateMachineGraphics statesG;

	GraphicsHandler* graphicsHandle;

	unsigned int textureID;

	Toolbar toolbar = Toolbar(states);
	Ribbons ribbons = Ribbons(states);
	Graphics graphics = Graphics(states, textureID);

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	GUIHandler(stateMachineGraphics* states, GraphicsHandler* graphicsHandler);

	// Function that handles which engine should be active.
	void renderGraphics();

	// Set custom theme.
	void setTheme();

};

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------