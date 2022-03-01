//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <memory>
#include "Application/Application.h"
#include "Application/Layers/Layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "External/Misc/ConsoleColor.h"
#include "Resources/ResourceHandler.h"
#include "GUI/Ribbon/Ribbon.h"
#include "GUI/Toolbar/toolbar.h"
#include "Lumen.h"

// Testing.
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Engines/EngineCore/EngineCore.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "Application/Layers/GuiLayer.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Base2DEngine/Base2DEngine.h"

#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Statics.																																	//
//==============================================================================================================================================//

Application* Application::m_instance = nullptr;

//==============================================================================================================================================//
//  Setup																																		//
//==============================================================================================================================================//

Application::Application(GLFWwindow* window) 
	: m_window(window)
{
	// Reset glfw time an application start.	
	glfwSetTime(0);

	// Set this instance as the singleton.
	Lumen::setApp(this);

	//  NOTE: TO BE DEPRECATED!
	m_guiState = std::make_unique<GUIState>();

	// Setup.
	glfwInitCallbacks();
	m_layerStack = std::make_unique<LayerStack>();
	m_eventLog = std::make_unique<EventLog>();

	// ImGui Inits.
	ImGuiIO& io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	setGuiTheme();

	// Initialisation frame.
	buildDocks();

	// Create the main GUI layers.
	pushGuiLayer<Toolbar>("Main Toolbar", DockPanel::Fixed);
	pushGuiLayer<Ribbon>("Main Ribbon", DockPanel::Ribbon);
}

void Application::shutdown() 
{
	// ImGUI cleanup.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();

	// Close application.
	glfwDestroyWindow(m_window);
	glfwTerminate();

	// Log termination.
	std::cout << blue << "\n\n[LUMEN] [INFO] : " << white << "Program terminated." << std::endl;
}

bool Application::shouldWindowClose()
{
	return m_shouldWindowClose;
}

void Application::closeWindow() 
{
	m_shouldWindowClose = true;
}

GLFWwindow* Application::getWindow()
{
	return m_window;
}

//==============================================================================================================================================//
//  GUI Theme.																																	//
//==============================================================================================================================================//

void Application::setGuiTheme()
{
	// ----------- //
	//  S T Y L E  //
	// ----------- //

	// Setup ImGui style.
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ItemSpacing = ImVec2(12, 6);
	style.ItemInnerSpacing = ImVec2(8, 4);
	style.IndentSpacing = 1.0f;
	style.ScrollbarSize = 15.0f;
	style.GrabMinSize = 5.0f;
	style.WindowMenuButtonPosition = ImGuiDir_Right;

	// Rounding.
	style.WindowRounding = 1.f;
	style.ChildRounding = 1.f;
	style.FrameRounding = 1.f;
	style.PopupRounding = 1.0f;
	style.TabRounding = 1.0f;
	style.ScrollbarRounding = 12.0f;
	style.GrabRounding = 12.0f;

	// Borders.
	style.TabBorderSize = 1.f;
	style.WindowBorderSize = 1.f;
	style.FrameBorderSize = 0.f;

	// Spacing.
	style.ItemInnerSpacing = ImVec2(1.f, 1.f);

	// Padding.
	style.DisplaySafeAreaPadding = ImVec2(0.f, 0.f);
	style.WindowPadding = ImVec2(10.f, 10.f);
	style.FramePadding = ImVec2(6.f, 3.5f);

	// ------------- //
	//  C O L O R S  //
	// ------------- //

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.81f, 0.82f, 0.90f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.09f, 0.12f, 0.90f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.81f, 0.82f, 0.90f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.33f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.29f, 0.29f, 0.38f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.41f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.16f, 0.16f, 0.19f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.63f, 0.64f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.98f, 1.00f, 0.83f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.25f, 0.26f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.71f, 0.72f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.50f, 0.51f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.25f, 0.26f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.00f, 0.50f, 0.51f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.81f, 0.82f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.17f, 0.17f, 0.19f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.10f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.10f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.48f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	// ------------------------- //
	//  L O A D I N G   F O N T  //
	// ------------------------- //

	ImGuiIO& io = ImGui::GetIO();
	void* fontPtr = getFontResourceMemoryLocation(ARIAL_NORMAL_TTF);
	unsigned fontSize = getFontResourceSize(ARIAL_NORMAL_TTF);
	m_defaultFont = io.Fonts->AddFontFromMemoryTTF(fontPtr, fontSize, 16.f);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//