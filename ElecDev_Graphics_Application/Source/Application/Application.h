#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <iostream>
#include <vector>
#include "Application/Events/EventLog.h"
#include "Application/Layers/LayerStack.h"
#include "Application/Layers/GuiLayer.h"
#include "Application/Layers/EngineLayer.h"
#include "imgui/imgui.h"
#include "imgui_internal.h"

// TO BE DEPRECATED.
#include "GuiState.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EventLog;
class Layer;
class ImFont;

struct GLFWwindow;

//==============================================================================================================================================//
//  Dock types.																																	//
//==============================================================================================================================================//

enum class DockPanel 
{
	Fixed,	// These panels are handled manually.
	Floating,
	Left,
	Right,
	Bottom,
	Scene
};

//==============================================================================================================================================//
//  Application class.																															//
//==============================================================================================================================================//

class Application
{

public:

	// Constructor.
	Application(GLFWwindow* window);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Renders the next frame.
	void onRender();
	
	// ------------- //
	//  L A Y E R S  //
	// ------------- //

	// Push an engine onto the layerstack.
	template<typename EngineType>
	EngineLayer<EngineType>* pushEngineLayer(std::string layerName,  DockPanel dockPanel = DockPanel::Scene, int imguiWindowFlags = 0);
	// Push a gui onto the layerstack.
	template<typename GuiType>
	GuiLayer<GuiType>* pushGuiLayer(std::string layerName, DockPanel dockPanel = DockPanel::Floating, int imguiWindowFlags = 0);
	// Pop a layer from the layerstack using the pointer.
	void queuePopLayer(Layer* layer);
	// Pop a layer from the layerstack using the layer name.
	void queuePopLayer(std::string& layerName);

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Handles all of the events in the event log.
	void dispatchEvents();
	// Handle events specifically for the Application layer.
	void onEvent(Event& event);
	// Log the event in the event log.
	template <typename EventType>
	void logEvent(Event& event);
	// Should the app close?
	bool shouldWindowClose();
	// Close the app.
	void closeWindow();
	// Get the GLFW window.
	GLFWwindow* getWindow();

	// ----------------------- //
	//  G L F W   W I N D O W  //
	// ----------------------- //

	// Sets up the GLFW window and OpenGL context.
	static GLFWwindow* glfwInitWindow();
	// Close the application.
	void shutdown();
	// Sets up the GLFW callbacks.
	void glfwInitCallbacks();

	// TO BE DEPRECATED!
	std::unique_ptr<GUIState> m_guiState;

private:

	friend class LayerStack;

	// The window containing the application.
	GLFWwindow* m_window;
	// A pointer to the application instance.
	// For use with Lumen::getApp()
	// HAS TO BE SINGLETON!
	static Application* m_instance;

	// ------------- //
	//  L A Y E R S  //
	// ------------- //
	
	// The layers in the application.
	std::unique_ptr<LayerStack> m_layerStack;
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
	Layer* findhoveredLayer();
	// Dock a layer to the panel.
	void dockLayerToPanel(std::string& name, DockPanel panel);

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
	// Should the app close?
	bool m_shouldWindowClose = false;
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Initialise the frame for rendering.
	void onRenderInit();
	// Cleanup after the frame has been rendered.
	void onRenderCleanup();
	// Renders the initial frame that is required for the dock builder.
	void buildDocks();
	// Set the ImGUI theme.
	void setGuiTheme();
	// The default font used.
	ImFont* m_defaultFont = nullptr;

	// --------------------- //
	//  D O C K   N O D E S  //
	// --------------------- //

	// The main window dockspace (minus the toolbar).
	ImGuiID m_mainDockspaceID = NULL;
	// The side panel where GUIs can displayed.
	ImGuiID m_leftPanelID = NULL;
	ImGuiID m_rightPanelID = NULL;
	ImGuiID m_bottomPanelID = NULL;
	// The area where the main graphics is displayed.
	ImGuiID m_scenePanelID = NULL;
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
EngineLayer<EngineType>* Application::pushEngineLayer(std::string layerName, DockPanel dockPanel, int imguiWindowFlags)
{
	// Create and push the layer.
	std::unique_ptr<EngineLayer<EngineType>> layer = std::make_unique<EngineLayer<EngineType>>(layerName, imguiWindowFlags);
	std::string newName = m_layerStack->pushLayer<EngineLayer<EngineType>>(layer);
	EngineLayer<EngineType>* ptr = m_layerStack->getLayer<EngineLayer<EngineType>>(newName);
	onFocusedLayerChange(ptr);
	// Dock the layer.
	dockLayerToPanel(newName, dockPanel);
	// Return the layer.
	return ptr;
}

template<typename GuiType>
GuiLayer<GuiType>* Application::pushGuiLayer(std::string layerName, DockPanel dockPanel, int imguiWindowFlags)
{
	// Create and push the layer.
	std::unique_ptr<GuiLayer<GuiType>> layer = std::make_unique<GuiLayer<GuiType>>(layerName, imguiWindowFlags);
	std::string newName = m_layerStack->pushLayer<GuiLayer<GuiType>>(layer);
	GuiLayer<GuiType>* ptr = m_layerStack->getLayer<GuiLayer<GuiType>>(newName);
	onFocusedLayerChange(ptr);
	// Dock the layer.
	dockLayerToPanel(newName, dockPanel);
	// Return the layer.
	return ptr;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//