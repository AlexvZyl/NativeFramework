#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include <../Graphics/graphicsHandler.h>

//  General.
#include <string>
#include <iostream>
#include <map>
#include <list>

#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "Graphics/graphics.h"
#include "UserGUI/userGUI.h"
#include "MCC/mcc.h"
#include "../stateMachine.h"

// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_glfw.h"
#include "Implementations/imgui_impl_opengl3.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

//----------------------------------------------------------------------------------------------------------------------
//  GUI Handler Class.
//----------------------------------------------------------------------------------------------------------------------


class GUIHandler
{
public:

	//--------------------------------------------------------------------------------------------------------------
	//  Variables.
	//--------------------------------------------------------------------------------------------------------------

	stateMachine* states;

	GraphicsHandler* graphicsHandler;

	unsigned int textureID;

	Toolbar* toolbar;
	Ribbons* ribbons;
	Graphics* graphics;
	userGUI* userGUIP;
	MCC* mcc;
	ImGuiID dock;
	ImGuiID dockSpaceID;

	boolean dockCreated = false;

	ImGuiID dock_up_id;
	ImGuiID dock_right_id;
	ImGuiID dock_left_id;
	ImGuiID dock_down_id;
	ImGuiID dock_none_id;

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	GUIHandler(stateMachine* states, GraphicsHandler* graphicsHandler);
	// Destructor.
	~GUIHandler();
	void deQueueInput();

	// Function that handles which engine should be active.
	void renderGui(ImGuiIO& io);

	// Set custom theme.
	void setTheme();

	void createDock(ImVec2 work_size);



};


//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------

