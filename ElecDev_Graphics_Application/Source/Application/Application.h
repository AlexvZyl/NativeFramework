#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include <iostream>
#include <vector>
#include "Application/Events/EventLog.h"
#include "Application/Layers/GuiLayer.h"
#include "Application/Layers/EngineLayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/notify/imgui_notify.h"
#include "Lumen.h"

// TO BE DEPRECATED?
#include "GuiState.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EventLog;
class Layer;
class ImFont;
class LumenWebSocket;
class LayerStack;

struct GLFWwindow;
struct RendererData;
struct ProfileResult;

//==============================================================================================================================================//
//  Data.																																		//
//==============================================================================================================================================//

enum class DockPanel 
{
	Floating,	// Undocked windows.
	Left,		// The left panel.
	Right,		// The right panel.
	Bottom,		// The bottom panel.
	Scene,		// The main scene panel where the graphics are displayed.

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
	
	// ------------- //
	//  L A Y E R S  //
	// ------------- //

	// Push an engine onto the layerstack.
	template<typename EngineType>
	EngineLayer<EngineType>* pushEngineLayer(std::string layerName,  DockPanel dockPanel = DockPanel::Scene, int imguiWindowFlags = 0, bool focus = true);
	// Push a gui onto the layerstack.
	template<typename GuiType>
	GuiLayer<GuiType>* pushGuiLayer(std::string layerName, DockPanel dockPanel = DockPanel::Floating, int imguiWindowFlags = 0, bool focus = true);
	// Pop a layer from the layerstack using the pointer.
	void queuePopLayer(Layer* layer);
	// Pop a layer from the layerstack using the layer name.
	void queuePopLayer(std::string& layerName);
	// Get the layer, given the name.
	template<class LayerType>
	LayerType* getLayer(std::string& name);
	// Find the layer that this engine belongs to.
	template<class EngineType>
	EngineLayer<EngineType>* getEngineLayer(EngineCore* engine);

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Handles all of the events in the event log.
	void onUpdate();
	// Handle events specifically for the Application layer.
	void onEvent(Event& event);
	// Log the event in the event log.
	template <typename EventType>
	void logEvent(Event& event);
	// Should the app close?
	bool isRunning();
	// Close the app.
	void stopRunning();
	// Get the GLFW window.
	GLFWwindow* getWindow();

	// ----------------------- //
	//  G L F W   W I N D O W  //
	// ----------------------- //

	// Sets up the GLFW window and OpenGL context.
	static GLFWwindow* glfwInitWindow();
	// Sets up the GLFW callbacks.
	void glfwInitCallbacks();

	// TO BE DEPRECATED!
	std::unique_ptr<GUIState> m_guiState = nullptr;

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

private:

	// Queue of scipts to be executed.
	std::vector<std::string> m_luaScripts;

	// --------------- //
	//  G E N E R A L  //
	// --------------- //

	// Friends.
	friend class LayerStack;
	friend class SettingsWidget;

	// The window containing the application.
	GLFWwindow* m_window = nullptr;

	// The active engine.
	EngineCore* m_activeEngine = nullptr;

	// --------- //
	//  L O O P  //
	// --------- //

	// Is the app running.
	bool m_isRunning = true;
	bool m_waitForEvents = true;
	double m_targetFPS = 60.f;
	double m_targetFrameTime = 1.f / m_targetFPS;
	double m_totalFrameTime = 0;
	double m_currentFrameTime = 0;
	double m_eventsTimeout = m_targetFrameTime * 2;
	// Update the current frame time.
	void updateFrametime();
	// Checks if the frame has to be started based on the frametime.
	bool startFrame();

	// ------------- //
	//  L A Y E R S  //
	// ------------- //
	
	// The layers in the application.
	std::unique_ptr<LayerStack> m_layerStack = nullptr;
	// The layer that has the most recent interaction.
	Layer* m_focusedLayer = nullptr;
	// The layer that the mouse is hovering over.
	Layer* m_hoveredLayer = nullptr;
	// Handle when the hovered layer changes.
	void onHoveredLayerChange(Layer* newLayer);
	// Handle when the focused layer changes.
	// This allows us to control when windows are focused
	// in imgui.
	void onFocusedLayerChange(Layer* newLayer);
	// Find the layer that is being hovered.
	Layer* findHoveredLayer();
	// Dock a layer to the panel.
	void dockLayerToPanel(std::string& name, DockPanel panel);
	// Pop the layers queued for removal.
	void popLayers();

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Log containing all of the events.
	std::unique_ptr<EventLog> m_eventLog;
	// Handle window events.
	void onWindowResizeEvent(WindowEvent& event);
	// Handle serialisation events.
	void onFileDropEvent(FileDropEvent& event);
	// Load files.
	void onFileLoadEvent(FileLoadEvent& event);
	// Save files.
	void onFileSaveEvent(FileSaveEvent& event);
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
	// Render the Lumen layers.
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
	ImGuiID m_scenePanelID = NULL;
	ImGuiID m_ribbonPanelID = NULL;
	ImGuiID m_bottomBarID = NULL;
};

//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template <typename EventType>
void Application::logEvent(Event& event)
{
	// Log event in the event log.
	m_eventLog->log<EventType>(event);
}

template<typename EngineType>
EngineLayer<EngineType>* Application::pushEngineLayer(std::string layerName, DockPanel dockPanel, int imguiWindowFlags, bool focus)
{
	// Create and push the layer.
	std::unique_ptr<EngineLayer<EngineType>> layer = std::make_unique<EngineLayer<EngineType>>(layerName, imguiWindowFlags);
	std::string newName = m_layerStack->pushLayer<EngineLayer<EngineType>>(layer);
	EngineLayer<EngineType>* ptr = m_layerStack->getLayer<EngineLayer<EngineType>>(newName);
	if(focus)
		onFocusedLayerChange(ptr);
	// Dock the layer.
	dockLayerToPanel(newName, dockPanel);
	// Return the layer.
	return ptr;
}

template<typename GuiType>
GuiLayer<GuiType>* Application::pushGuiLayer(std::string layerName, DockPanel dockPanel, int imguiWindowFlags, bool focus)
{
	// Create and push the layer.
	std::unique_ptr<GuiLayer<GuiType>> layer = std::make_unique<GuiLayer<GuiType>>(layerName, imguiWindowFlags);
	std::string newName = m_layerStack->pushLayer<GuiLayer<GuiType>>(layer);
	GuiLayer<GuiType>* ptr = m_layerStack->getLayer<GuiLayer<GuiType>>(newName);
	if(focus)
		onFocusedLayerChange(ptr);
	// Dock the layer.
	dockLayerToPanel(newName, dockPanel);
	// Return the layer.
	return ptr;
	return nullptr;
}

template<class LayerType>
LayerType* Application::getLayer(std::string& name)
{
	return m_layerStack->getLayer<LayerType>(name);
}

template<class EngineType>
EngineLayer<EngineType>* Application::getEngineLayer(EngineCore* engine)
{
	// Find the layer with the engine.
	for (auto& [name, layer] : m_layerStack->getLayers())
	{
		EngineLayer<EngineType>* engineLayer = dynamic_cast<EngineLayer<EngineType>*>(layer.get());
		// Valid cast?
		if (!engineLayer) continue;
		// Does it contain the engine?
		if (engineLayer->getEngine() == engine) return engineLayer;
	}
	// Engine not found.
	return nullptr;
}

template<class EngineType>
EngineType* Application::getActiveEngine() 
{
	return dynamic_cast<EngineType*>(m_activeEngine);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//