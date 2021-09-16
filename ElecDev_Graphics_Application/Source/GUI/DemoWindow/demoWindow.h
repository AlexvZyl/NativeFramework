#pragma once
#include <Core/imgui.h>
#include <string>
#include "../stateMachine.h"
#include <../Graphics/graphicsHandler.h>
#include <GLFW/glfw3.h>
class demoWindow
{
public:

	//Variables
	stateMachine* states;
	ImVec2 pos;
	GraphicsHandler* graphicsHandler;
	ImGuiID dock;

	//Constructor
	demoWindow(stateMachine* states, GraphicsHandler* graphicsHandler);

	//Renderer
	void renderDemo(ImGuiID dock);
};

