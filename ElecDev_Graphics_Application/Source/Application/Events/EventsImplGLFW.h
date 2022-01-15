#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  GLFW implementation.																														//
//==============================================================================================================================================//

// Initialises GLFW.
GLFWwindow* initWindow();

// The GLFW callbacks.
void glfwErrorCallback(int error, const char* description);
void glfwMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
void glfwMouseMoveEvent(GLFWwindow* window, double xpos, double ypos);
void glfwMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
void glfwKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwFileDropEvent(GLFWwindow* window, int count, const char** paths);

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
