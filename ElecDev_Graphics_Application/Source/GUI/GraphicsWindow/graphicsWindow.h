#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
// Include GLFW (window) after OpenGL definition.
#include <GLFW/glfw3.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>


#pragma once
class GraphicsWindow
{

public:


	// Constructor.
	GraphicsWindow();
	void renderGraphics(GLFWwindow* window);
};

