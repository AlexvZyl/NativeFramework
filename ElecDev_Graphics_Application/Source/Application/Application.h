#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include <iostream>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "imgui/notify/imgui_notify.h"
#include "Lumen.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Application/LumenWindow/LumenWindow.h"
#include "Application/LumenWindow/WindowStack.h"
#include "GUI/GraphicsScene/GraphicsScene.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class LumenWebSocket;
class EngineCore;
class WindowEvent;
class FileDropEvent;
class Event;
class FileLoadEvent;
class FileSaveEvent;
class AssetViewer;

template<class EngineType>
class GraphicsScene;

struct ImFont;
struct GLFWwindow;
struct RendererData;
struct ProfileResult;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

enum class LumenDockPanel
{
	Floating,	// Undocked windows.
	Left,		// The left panel.
	Right,		// The right panel.
	Bottom,		// The bottom panel.
	Scene,		// The main scene panel where the graphics are displayed.
	AssetViewer,// Node used for the asset viewer.

	// DO NOT USE!  Internal use only.
	Ribbon,		// The left ribbon that holds the buttons).
	Fixed,		// These panels are handled manually).
};

enum class NotificationType
{
	Success,
	Warning,
	Error,
	Info,
};

//==============================================================================================================================================//
//  Application class.																															//
//==============================================================================================================================================//

class Application
{

public:

	// Constructor.
	Application();

	// Destructor.
	~Application();

	// Run the application;
	void run();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Renders the next frame.
	void renderFrame();
	// Get the delta time for the current frame.
	// This is updated when a new frame start.
	inline float getDeltaTime() const { return m_deltaTime; }
	
	// --------------- //
	//  W I N D O W S  //
	// --------------- //

	// Push a window onto Lumen's window stack.
	template<class WindowType, class ... Args>
	WindowType* pushWindow(LumenDockPanel panel, const Args& ... args);

	// Push an engine onto Lumen's window stack.
	template<class EngineType, class ... Args>
	EngineType* pushEngine(LumenDockPanel panel, const std::string& name, const Args& ... args);
	
	// Queue a window to be popped.
	void queueWindowPop(LumenWindow* window);
	// Queue a window to be popped.
	void queueWindowPop(unsigned ID);

	// Get the asset viewer engine.
	inline AssetViewer* getAssetViewerEngine() { return m_assetViewerEngine; };

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Handles all of the events in the event log.
	void onUpdate();
	// Handle events specifically for the Application Window.
	void onEvent(const Event& event);
	// Should the app close?
	bool isRunning();
	// Close the app.
	void stopRunning();

	// ----------------------- //
	//  G L F W   W I N D O W  //
	// ----------------------- //

	// Sets up the GLFW window and OpenGL context.
	static GLFWwindow* glfwInitWindow();
	// Sets up the GLFW callbacks.
	void glfwInitCallbacks();
	// Get the GLFW window.
	GLFWwindow* getGLFWWindow() const;
	// Get the size of the application.
	glm::vec2 getMainViewportSize() const;

	// ----------------- //
	//  P R O F I L E R  //
	// ----------------- //

	// The results from the profiler.
	std::vector<ProfileResult> m_profilerResults;
	bool m_profilerActive = false;
	RendererData m_rendererData;

	// --------------------------- //
	//  N O T I F I C A T I O N S  //
	// --------------------------- //

	// Push a notification on top in Lumen.
	void pushNotification(NotificationType type, int msTime, const std::string& content, const std::string& title = "");

	// ------- //
	//  L U A  //
	// ------- //

	// Execute the scripts in the queue.
	void executeLuaScriptQueue();
	// Add a Lua script to the queue.
	void pushLuaScript(const std::string& script);

	// Set the active engine.
	void setActiveEngine(EngineCore* engine);
	// Get the base pointer active engine.
	EngineCore* getActiveEngine();
	// Get the active engine of a specific type.
	// Returns nullptr if it is not the same type.
	template<class EngineType>
	EngineType* getActiveEngine();

	// --------------------------- //
	//  A S S E T   V I E W I N G  //
	// --------------------------- //

	void viewCircuit(const std::filesystem::path& path);
	void viewCircuit(const YAML::Node& path);
	void viewComponent(const std::filesystem::path& path);
	void viewComponent(const YAML::Node& path);
	void viewAsset(const std::filesystem::path& path);

private:

	// Queue of scipts to be executed.
	std::vector<std::string> m_luaScripts;

	// --------------- //
	//  G E N E R A L  //
	// --------------- //

	// Friends.
	friend class WindowStack;
	friend class SettingsWidget;
	friend class RendererStats;
	friend class LumenWindow;
	friend class Toolbar;

	// The window containing the application.
	GLFWwindow* m_glfwWindow = nullptr;

	// The active engine.
	EngineCore* m_activeEngine = nullptr;

	// The engine used to view assets.
	AssetViewer* m_assetViewerEngine = nullptr;

	// --------- //
	//  L O O P  //
	// --------- //

	// Data.
	bool m_isRunning = true;
	bool m_waitForEvents = true;
	double m_targetFPS = 60.f;
	double m_targetFrameTime = 1.f / m_targetFPS;
	double m_totalFrameTime = 0;
	double m_currentFrameTime = 0;
	double m_eventsTimeout = m_targetFrameTime * 2;
	float m_deltaTime = 0.f;

	// Update the current frame time.
	void updateFrametime();
	// Checks if the frame has to be started based on the frametime.
	bool startFrame();

	// ------------- //
	//  W I N O W S  //
	// ------------- //
	
	// The Windows in the application.
	std::unique_ptr<WindowStack> m_windowStack = nullptr;
	// The Window that has the most recent interaction.
	LumenWindow* m_focusedWindow = nullptr;
	// The Window that the mouse is hovering over.
	LumenWindow* m_hoveredWindow = nullptr;
	// Handle when the hovered Window changes.
	void onHoveredWindowChange(LumenWindow* newWindow);
	// Handle when the focused Window changes.
	void onFocusedWindowChange(LumenWindow* newWindow);
	// Find the Window that is being hovered.
	LumenWindow* findHoveredWindow();
	// Dock a Window to the panel.
	void dockWindowToPanel(LumenWindow* window, LumenDockPanel panel);
	// Pop the Windows queued for removal.
	void popWindows();
	
	// Functions used to get data regarding the docking child nodes.
	ImGuiID findLargestChildNode(ImGuiID nodeID);
	ImGuiID findLastActiveChildNode(ImGuiID nodeID);
	void findChildNodes(ImGuiDockNode* currentNode, std::vector<ImGuiDockNode*>& nodes);

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	void onWindowResizeEvent(const WindowEvent& event);
	void onFileDropEvent(const FileDropEvent& event);
	void onFileLoadEvent(const FileLoadEvent& event);
	void onFileSaveEvent(const FileSaveEvent& event);
	// Update the ImGui state.
	// Lumen controls some of the state changes (for optimisation).
	void imguiOnUpdate();
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Initialise the frame for rendering.
	void onRenderInit();
	// Cleanup after the frame has been rendered.
	void onRenderCleanup();
	// Render the Lumen Windows.
	void onRender();
	// Renders the initial frame that is required for the dock builder.
	void buildDocks();
	// Set the ImGUI theme.
	void setGuiTheme();
	// The default font used.
	ImFont* m_defaultFont = nullptr;

	// --------------------- //
	//  C O N N E C T I O N  //
	// --------------------- //

	std::unique_ptr<LumenWebSocket> m_webSocket = nullptr;

	// --------------------- //
	//  D O C K   N O D E S  //
	// --------------------- //

	// Dock space IDs.
	ImGuiID m_mainDockspaceID = NULL;
	ImGuiID m_leftPanelID = NULL;
	ImGuiID m_rightPanelID = NULL;
	ImGuiID m_bottomPanelID = NULL;
	ImGuiID m_bottomAssetViewerID = NULL;
	ImGuiID m_scenePanelID = NULL;
	ImGuiID m_ribbonPanelID = NULL;
	ImGuiID m_bottomBarID = NULL;
};

//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template<class WindowType, class ... Args>
WindowType* Application::pushWindow(LumenDockPanel panel, const Args& ... args)
{
	WindowType* window = m_windowStack->pushWindow<WindowType>(args...);
	dockWindowToPanel(window, panel);
	window->focus();
	return window;
}

template<class EngineType, class ... Args>
EngineType* Application::pushEngine(LumenDockPanel panel, const std::string& name, const Args& ... args) 
{
	GraphicsScene<EngineType>* window = m_windowStack->pushWindow<GraphicsScene<EngineType>>(name);
	dockWindowToPanel(window, panel);
	window->constructEngine(args...);
	window->focus();
	return window->getEngine();
}

template<class EngineType>
EngineType* Application::getActiveEngine() 
{
	return dynamic_cast<EngineType*>(m_activeEngine);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//