#pragma once
#include <Core/imgui.h>
#include <queue>
#include <string>

struct inputQueue {

	//Constructor
	inputQueue(std::string command, std::string parameters) : command(command), parameters(parameters)
	{}
	std::string command;
	std::string parameters;
};

struct stateMachine
{
	bool toolsExpanded;
	int toolsMode; //0 = nothing, 1 = Draw MCC, 2 = Block Diagram, 3 = Draw Circuit Bucket
	bool renderWindowHovered;
	bool renderResizeEvent;
	ImVec2 renderWindowMouseCoordinate;
	ImVec2 renderWindowSize;

	bool startMainGraphics = true;

	bool startDemoWindow = false;

	bool showGraphicsWindow = true;

	bool globalQuit = false;

	std::queue<inputQueue> inputQueue;

};

struct mccStruct {

	ImVec2* vieportDimentions;
	bool* isHovered;
	BaseEngineGL* engine;


};

