#pragma once
#include <Core/imgui.h>

struct stateMachine
{
	bool toolsExpanded;
	int toolsMode; //0 = nothing, 1 = Draw MCC, 2 = Block Diagram, 3 = Draw Circuit Bucket
	bool renderWindowHovered;
	bool renderResizeEvent;
	ImVec2 renderWindowMouseCoordinate;

};