#include "demoWindow.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>


// Constructor.
demoWindow::demoWindow(stateMachine* states, GraphicsHandler* graphicsHandler)
	: states(states), graphicsHandler(graphicsHandler)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;
}