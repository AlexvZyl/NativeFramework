#pragma once

struct stateMachine
{
	bool toolsExpanded;
	int toolsMode; //0 = nothing, 1 = Draw MCC, 2 = Block Diagram, 3 = Draw Circuit Bucket

};

struct stateMachineGraphics {

	int mode;
	bool gui;
};