//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

//  General.
#include <string>
#include <iostream>

#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "../Helper/stateMachine.h"
#include <../Graphics/graphicsHandler.h>



#pragma once
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

	GraphicsHandler graphics;

	Toolbar toolbar = Toolbar(states);
	Ribbons ribbons = Ribbons(states);

	

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	GUIHandler(stateMachineGraphics* states, GraphicsHandler graphicsIn);

	// Function that handles which engine should be active.
	void renderGraphics();

};
