//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/WebSocket/LumenWebSocket.h"
#include <iostream>
#include <memory>
#include "Application/Application.h"
#include "Application/Layers/Layer.h"
#include "Application/Layers/GuiLayer.h"
#include "Application/Events/EventLog.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/notify/imgui_notify.h"
#include "Resources/ResourceHandler.h"
#include "GUI/Ribbon/Ribbon.h"
#include "GUI/Toolbar/toolbar.h"
#include "GUI/AssetExplorer/AssetExplorer.h"
#include "GUI/BottomBar/BottomBar.h"
#include "Lumen.h"
#include "Utilities/Profiler/Profiler.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Logger/Logger.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Setup																																		//
//==============================================================================================================================================//

Application::Application() 
{
	LUMEN_LOG_INFO("Application started!", "");

	// Create GLFW window.
	m_window = Application::glfwInitWindow();

	// Set this instance as the singleton.
	Lumen::setApp(this);

	// Events & Layers.
	Application::glfwInitCallbacks();
	EventLog::init();
	m_layerStack = std::make_unique<LayerStack>();

	// NOTE: TO BE DEPRECATED!
	m_guiState = std::make_unique<GUIState>();

	// ImGui Inits.
	ImGuiIO& io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	setGuiTheme();

	// Initialisation frame.
	buildDocks();

	// Create the main GUI layers.
	auto* toolbar = pushGuiLayer<Toolbar>("Main Toolbar", LumenDockPanel::Fixed)->getGui();
	toolbar->m_assetExplorerLayer = pushGuiLayer<AssetExplorer>("Asset Explorer", LumenDockPanel::Bottom, 0, false);
	pushGuiLayer<Ribbon>("Main Ribbon", LumenDockPanel::Ribbon, 0, false);
	pushGuiLayer<BottomBar>("Bottom Bar", LumenDockPanel::Fixed, 0, false);

	// Create web socket and give some time to setup.
	m_webSocket = std::make_unique<LumenWebSocket>();

	// Flush the buffer after the app has started.
	// This allows external programs (python server) to read.
	std::cout.flush();
}

Application::~Application()
{
	// Clear layers.
	m_layerStack->getLayers().clear();

	// ImGUI cleanup.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// GLFW cleanup.
	glfwDestroyWindow(m_window);
	glfwTerminate();

	// Log termination.
	LUMEN_LOG_INFO("Program terminated.", "");
}

bool Application::isRunning()
{
	return m_isRunning;
}

void Application::stopRunning() 
{
	m_isRunning = false;
}

GLFWwindow* Application::getWindow()
{
	return m_window;
}

void Application::setActiveEngine(EngineCore* engine) 
{
	m_activeEngine = engine;
}

EngineCore* Application::getActiveEngine()
{
	return m_activeEngine;
}

//==============================================================================================================================================//
//  Lua																																			//
//==============================================================================================================================================//

void Application::executeLuaScriptQueue() 
{
	if (!m_luaScripts.size()) return;

	// Execute Lua scripts.
	for (auto& script : m_luaScripts)
	{
		lua_State* L = lua_CreateNewLuaState();
		lua_ExecuteScript(L, script, true);
		lua_close(L);
	}
	m_luaScripts.clear();
}

void Application::pushLuaScript(const std::string& script)
{
	m_luaScripts.push_back(script);
}

//==============================================================================================================================================//
//  GUI Theme.																																	//
//==============================================================================================================================================//

void Application::setGuiTheme()
{
	// Used for initial setup.
	static bool first = true;

	// ----------- //
	//  S T Y L E  //
	// ----------- //


	if (first)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::StyleColorsDark();
		style.ItemSpacing = ImVec2(12, 6);
		style.ItemInnerSpacing = ImVec2(8, 4);
		style.IndentSpacing = 1.0f;
		style.ScrollbarSize = 15.0f;
		style.GrabMinSize = 5.0f;
		style.WindowMenuButtonPosition = ImGuiDir_Right;

		// Rounding.
		style.WindowRounding = 1.f;
		style.ChildRounding = 1.f;
		style.FrameRounding = 2.f;
		style.PopupRounding = 1.0f;
		style.TabRounding = 2.0f;
		style.ScrollbarRounding = 12.0f;
		style.GrabRounding = 12.0f;

		// Borders.
		style.TabBorderSize = 1.f;
		style.WindowBorderSize = 1.f;
		style.FrameBorderSize = 0.f;

		// Spacing.
		style.ItemInnerSpacing = ImVec2(1.f, 1.f);
		style.ItemSpacing = {5.f, 4.f};

		// Padding.
		style.DisplaySafeAreaPadding = ImVec2(0.f, 0.f);
		style.WindowPadding = ImVec2(6.f, 6.f);
		style.FramePadding = ImVec2(6.f, 3.5f);

		// AA.
		style.AntiAliasedLinesUseTex = true;
		style.AntiAliasedFill = true;
		style.AntiAliasedLines = true;
	}

	// ------------- //
	//  C O L O R S  //
	// ------------- //
	
	ImVec4* colors = ImGui::GetStyle().Colors;
	if (first)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.18f, 0.30f, 0.67f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.16f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.36f, 0.39f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.09f, 0.60f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.45f, 0.45f, 0.49f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.57f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.61f, 0.61f, 0.65f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.18f, 0.30f, 0.67f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.57f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.61f, 0.61f, 0.65f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.27f, 0.30f, 0.36f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.36f, 0.44f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.44f, 0.52f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.18f, 0.19f, 0.23f, 0.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.32f, 0.33f, 0.39f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.38f, 0.45f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.30f, 0.67f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.38f, 0.54f, 1.00f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.54f, 1.00f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.18f, 0.30f, 0.67f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.30f, 0.67f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.18f, 0.30f, 0.67f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.24f, 0.24f, 0.26f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.03f, 0.03f, 0.03f, 0.35f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.03f, 0.03f, 0.03f, 0.56f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.18f, 0.30f, 0.67f, 0.50f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}

	// --------------------------- //
	//  M O D I F I C A T I O N S  //
	// --------------------------- //

	colors[ImGuiCol_SeparatorHovered] = colors[ImGuiCol_Separator];
	colors[ImGuiCol_Border] = colors[ImGuiCol_Separator];
	colors[ImGuiCol_ScrollbarBg] = colors[ImGuiCol_Separator];
	colors[ImGuiCol_ScrollbarBg].w = 0.6f;
	colors[ImGuiCol_DockingEmptyBg] = colors[ImGuiCol_Separator];

	colors[ImGuiCol_MenuBarBg] = colors[ImGuiCol_WindowBg];
	colors[ImGuiCol_TitleBg] = colors[ImGuiCol_WindowBg];
	colors[ImGuiCol_Tab] = colors[ImGuiCol_WindowBg];
	colors[ImGuiCol_TitleBgActive] = colors[ImGuiCol_WindowBg];
	colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_WindowBg];
	colors[ImGuiCol_PopupBg] = colors[ImGuiCol_WindowBg];
	colors[ImGuiCol_TitleBgCollapsed] = colors[ImGuiCol_WindowBg];

	colors[ImGuiCol_CheckMark] = colors[ImGuiCol_TextDisabled];
	colors[ImGuiCol_SeparatorActive] = colors[ImGuiCol_TextDisabled];
	colors[ImGuiCol_TabActive] = colors[ImGuiCol_TextDisabled];
	colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TextDisabled];
	colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_TextDisabled];
	colors[ImGuiCol_TextSelectedBg] = colors[ImGuiCol_TextDisabled];
	colors[ImGuiCol_TextSelectedBg].w = 0.5f;

	// ------------------------- //
	//  L O A D I N G   F O N T  //
	// ------------------------- //

	if (first)
	{
		first = false;

		// Tell imgui that it does not own the font memory data.
		ImFontConfig imFontConfig;
		imFontConfig.FontDataOwnedByAtlas = false;

		ImGuiIO& io = ImGui::GetIO();
		// Load the font from the .exe memory.
		void* fontPtr = getFontResourceMemoryLocation(ARIAL_NORMAL_TTF);
		unsigned fontSize = getFontResourceSize(ARIAL_NORMAL_TTF);
		m_defaultFont = io.Fonts->AddFontFromMemoryTTF(fontPtr, fontSize, 16.f, &imFontConfig);

		// Init notify.
		ImGui::MergeIconsWithLatestFont(16.f, false);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//