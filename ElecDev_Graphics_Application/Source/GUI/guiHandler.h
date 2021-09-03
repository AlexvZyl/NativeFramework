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
#include "../Helper/stateMachine.h"

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

	GraphicsHandler* graphics;

	Toolbar toolbar = Toolbar(states);
	Ribbons ribbons = Ribbons(states);

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	GUIHandler(stateMachineGraphics* states, GraphicsHandler* graphicsHandler);

	// Function that handles which engine should be active.
	void renderGraphics();

};
