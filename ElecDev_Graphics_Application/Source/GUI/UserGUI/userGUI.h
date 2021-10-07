#pragma once
#include <Core/imgui.h>
#include <string>
#include "../stateMachine.h"
#include <../Graphics/graphicsHandler.h>
#include <GLFW/glfw3.h>

class userGUI
{
public:

	//Variables
	stateMachine* states;
	ImVec2 pos;
	GraphicsHandler* graphicsHandler;
	ImGuiID dock;
	bool userPrevious;
	bool close = true;
	const char* voltageLevel = "460";
	const char* efficiencyRating = "IEC";
	char steadyVoltDrop[128] = "Hello, world!";

	//Constructor
	userGUI(stateMachine* states, GraphicsHandler* graphicsHandler);

	//Renderer
	void renderWindow(ImGuiID dock);
	void deQueueInput();
};