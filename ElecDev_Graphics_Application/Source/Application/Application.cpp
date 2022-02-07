//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <memory>
#include "Application/Application.h"
#include "Application/Events/Events.h"
#include "Application/Events/EventLog.h"
#include "Application/Layers/LayerStack.h"
#include "Application/Layers/Layer.h"
#include "External/ImGUI/Core/imgui.h"
#include "External/ImGUI/Implementations/imgui_impl_glfw.h"
#include "External/ImGUI/Implementations/imgui_impl_opengl3.h"
#include "External/Misc/ConsoleColor.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/RendererGL.h"
#include "Resources/ResourceHandler.h"

// Testing.
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "GUI/GraphicsScene/GraphicsScene.h"
#include "Engines/EngineCore/EngineCore.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/CameraGL.h"
#include "Application/Layers/BasicGuiLayer.h"
#include "Application/Layers/EngineLayer.h"

#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Setup																																		//
//==============================================================================================================================================//

Application::Application(GLFWwindow* window) 
	: m_window(window)
{
	glfwInitCallbacks();
	m_layerStack = std::make_unique<LayerStack>();
	m_eventLog = std::make_unique<EventLog>();

	// Testing layers.
	BasicGuiLayer guiLayer(LayerType_ComponentEditor, "Back");
	m_layerStack->pushLayerToFront<BasicGuiLayer>(guiLayer);
	BasicGuiLayer guiLayer2(LayerType_ComponentEditor, "Middle");
	m_layerStack->pushLayerToFront<BasicGuiLayer>(guiLayer2);
	EngineLayer guiLayer4(LayerType_Design2DEngine, "Front");
	m_layerStack->pushLayerToFront<EngineLayer>(guiLayer4);

	// ImGui Inits.
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	setGuiTheme();
}

void Application::shutdown() 
{
	// ImGUI cleanup.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();  // This causes an exception?

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

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

void Application::onRender() 
{
	// Init.
	onRenderInit();

	// Render each layer from back to front.
	for (std::unique_ptr<Layer>& layer : m_layerStack->getLayers())
		layer->onRender();

	// Cleanup.
	onRenderCleanup();
}

void Application::onRenderInit() 
{
	// Feed inputs to ImGUI, start new frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Enable main viewport docking.
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	// Font.
	ImGui::PushFont(m_defaultFont);
}

void Application::onRenderCleanup() 
{
	// Font.
	ImGui::PopFont();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Assign values to viewport for ImGUI.
	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	glm::vec2 viewport(display_w, display_h);
	Renderer::setViewport(viewport);

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Application::swapBuffers() 
{
	glfwSwapBuffers(m_window);
}

//==============================================================================================================================================//
//  GUI Theme.																																	//
//==============================================================================================================================================//

void Application::setGuiTheme()
{
	// Setup ImGui style.
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 5.0f;
	style.WindowRounding = 5.0f;
	style.ItemSpacing = ImVec2(12, 6);
	style.ItemInnerSpacing = ImVec2(8, 4);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;
	style.ChildRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.PopupRounding = 5.0f;
	style.WindowPadding = ImVec2(1.f, 1.f);
	style.WindowBorderSize = 0.f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.808f, 0.824f, 0.9f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.48f);

	colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

	colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.150f, 0.154f, 0.5f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);

	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.57f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
	colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.416f, 0.424f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.41f, 0.42f, 0.46f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.41f, 0.42f, 1.00f);

	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);

	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);

	colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	colors[ImGuiCol_Button] = ImVec4(0.0f, 0.490f, 0.498f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.612f, 0.624f, 0.83f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.68f, 0.42f, 0.58f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.150f, 0.154f, 1.0f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	colors[ImGuiCol_Separator] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.36f, 0.75f, 0.10f, 1.00f);

	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.808f, 0.824f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

	colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);

	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

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