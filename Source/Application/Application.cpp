//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/WebSocket/LumenWebSocket.h"
#include <iostream>
#include <memory>
#include "Application/Application.h"
#include "Application/Windows/LumenWindow.h"
#include "Application/Windows/WindowStack.h"
#include "Application/Events/EventLog.h"
#include "Application/ApplicationTemplates.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/notify/imgui_notify.h"
#include "Resources/ResourceHandler.h"
#include "GUI/Ribbon/Ribbon.h"
#include "GUI/Toolbar/toolbar.h"
#include "GUI/AssetExplorer/AssetExplorer.h"
#include "GUI/BottomBar/BottomBar.h"
#include "Lumen/Lumen.h"
#include "Utilities/Profiler/Profiler.h"
#include "Utilities/Lua/LuaInterpreter.h"
#include "External/Misc/ConsoleColor.h"
#include "Utilities/Logger/Logger.h"
#include "GLFW/glfw3.h"
#include "Engines/AssetViewer/AssetViewer.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Graphics/Entities/EntityManager.h"

//==============================================================================================================================================//
//  Setup																																		//
//==============================================================================================================================================//

Application::Application() 
{
	LUMEN_LOG_INFO("Application started!", "");

	// Create GLFW window.
	m_glfwWindow = Application::glfwInitWindow();

	// Init systems.
	Renderer::init();
	EntityManager::init();
	Application::glfwInitCallbacks();
	EventLog::init();
	Lumen::setApp(this);
	// Give imgui some frames to startup.
	startWindowResizing(120);
	m_windowStack = std::make_unique<WindowStack>();

	// ImGui setup.
	ImGuiIO& io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigInputTrickleEventQueue = false;
	setGuiTheme();
	setCursorMode(CursorMode::OS);

	// Initialisation frame.
	buildDocks();

	// Create the main (default) GUI windows.
	Toolbar* toolbar = pushWindow<Toolbar>(LumenDockPanel::Fixed, "Main Toolbar");
	auto* assetViewer = pushWindow<AssetExplorer>(LumenDockPanel::Bottom, "Asset Explorer");
	toolbar->m_assetExplorerWindow = assetViewer;
	m_assetViewerEngine = assetViewer->m_assetViewerEngine;
	pushWindow<Ribbon>(LumenDockPanel::Ribbon, "Main Ribbon");
	pushWindow<BottomBar>(LumenDockPanel::Fixed, "Bottom Bar");

	// Create web socket and give some time to setup.
	m_webSocket = std::make_unique<LumenWebSocket>();
	Sleep(10); // No
}

Application::~Application()
{
	// Clear windows.
	m_windowStack->clear();

	// ImGUI cleanup.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Shutdown renderer.
	Renderer::shutdown();

	// GLFW cleanup.
	glfwDestroyWindow(getGLFWWindow());
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

GLFWwindow* Application::getGLFWWindow() const
{
	return m_glfwWindow;
}

glm::vec2 Application::getMainViewportSize() const 
{
	int display_w, display_h;
	glfwGetFramebufferSize(getGLFWWindow(), &display_w, &display_h);
	return { display_w, display_h };
}

void Application::setActiveEngine(EngineCore* engine) 
{
	m_activeEngine = engine;
}

EngineCore* Application::getActiveEngine()
{
	return m_activeEngine;
}

void Application::pushEngine(EngineCore* engine)
{
	m_engines.push_back(engine);
}

void Application::popEngine(EngineCore* engine) 
{
	int index = 0;
	for(auto* e : getEnignes())
	{
		if (engine == e) break;
		index++;
	}
	LUMEN_DEBUG_ASSERT(index < getEnignes().size(), "Could not find engine.");
	m_engines.erase(m_engines.begin() + index);
}

std::vector<EngineCore*> Application::getEnignes()
{
	return m_engines;
}

void Application::setCursorMode(CursorMode mode) 
{
	if (m_currentCursormode == mode) return;

	m_currentCursormode = mode;

	switch (mode) 
	{
	case CursorMode::OS:
		ImGui::GetIO().MouseDrawCursor = false;
		glfwSetInputMode(getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;

	case CursorMode::ImGui:
		ImGui::GetIO().MouseDrawCursor = true;
		glfwSetInputMode(getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;

	default:
		LUMEN_LOG_ERROR("Unknown cursor mode.", "Application");
		break;
	}
}

//==============================================================================================================================================//
//  Asset viewing.																																//
//==============================================================================================================================================//

void Application::viewCircuit(const std::filesystem::path& path)
{
	if (!m_assetViewerEngine) return;
	m_assetViewerEngine->viewCircuit(path);
}

void Application::viewCircuit(const YAML::Node& path)
{
	if (!m_assetViewerEngine) return;
	m_assetViewerEngine->viewCircuit(path);
}

void Application::viewComponent(const std::filesystem::path& path)
{
	if (!m_assetViewerEngine) return;
	m_assetViewerEngine->viewComponent(path);
}

void Application::viewComponent(const YAML::Node & path)
{
	if (!m_assetViewerEngine) return;
	m_assetViewerEngine->viewComponent(path);
}

void Application::viewAsset(const std::filesystem::path& path) 
{
	if (!m_assetViewerEngine) return;
	m_assetViewerEngine->viewAsset(path);
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
	// This allows us to continuously update the them without breaking it.
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
		colors[ImGuiCol_Border] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.60f);
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
		colors[ImGuiCol_Separator] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
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
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.00f, 0.00f, 0.00f, 0.22f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.35f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09f, 0.09f, 0.09f, 0.35f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.18f, 0.30f, 0.67f, 0.50f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
	}

	// --------------------------- //
	//  M O D I F I C A T I O N S  //
	// --------------------------- //

	colors[ImGuiCol_SeparatorHovered] = colors[ImGuiCol_Separator];
	colors[ImGuiCol_Border] = colors[ImGuiCol_Separator];
	colors[ImGuiCol_ScrollbarBg] = colors[ImGuiCol_Separator];
	colors[ImGuiCol_ScrollbarBg].w = 0.6f;
	colors[ImGuiCol_DockingEmptyBg] = colors[ImGuiCol_Separator];
	
	colors[ImGuiCol_MenuBarBg] = (glm::vec4)colors[ImGuiCol_WindowBg] / 1.35f;
	colors[ImGuiCol_MenuBarBg].w = 1.f;
	colors[ImGuiCol_TitleBg] = colors[ImGuiCol_MenuBarBg];
	colors[ImGuiCol_TitleBgActive] = colors[ImGuiCol_MenuBarBg];

	colors[ImGuiCol_Tab] = colors[ImGuiCol_WindowBg];
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

	first = false;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//