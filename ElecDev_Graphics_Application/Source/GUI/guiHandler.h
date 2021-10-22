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

struct element {
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

struct guiHolder {
	std::string windowName;
	ImVec2 windowPos;
	std::list<element> elements;
	bool close = true;
	bool submit = false;
	bool docked = false;

	guiHolder(std::string window, ImVec2 windowPosIn, std::list<element> inElements) {
		windowName = window;
		elements = inElements;
		windowPos = windowPosIn;
	}
};


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
	userGUI* userWindow;
	MCC* mcc;
	std::list<guiHolder> guis;

	//--------------------------------------------------------------------------------------------------------------
	//  Functions.
	//--------------------------------------------------------------------------------------------------------------

	// Constructor.
	GUIHandler(stateMachine* states, GraphicsHandler* graphicsHandler);
	// Destructor.
	~GUIHandler();
	void deQueueInput();

	// Function that handles which engine should be active.
	void renderGui(ImGuiIO& io, GLFWwindow* window);

	// Set custom theme.
	void setTheme();

	void createGUI(std::string guiName, std::string guiPos, std::string parameters);

	void renderUI(ImGuiID dock);

	void pushData(std::list<guiHolder>::iterator uiWindow);

};


//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------

