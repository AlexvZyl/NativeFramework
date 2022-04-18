#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "Application/Events/Events.h"

//==============================================================================================================================================//
//  Forward Declerations.																														//
//==============================================================================================================================================//

struct ImGuiWindow;

class Event;
class WindowEvent;

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

// LumenWindow is a wrapper class around an ImGuiWindow so that 
// Lumen can work with it in an OOP fashion.
class LumenWindow
{
public:

	// Constructor.
	// IMPORTANT: onAttach() has to be called when the layer is pushed onto the stack.
	inline LumenWindow(const char* name, int imguiWindowFlags = 0)
		: m_windowName(name), m_imguiWindowFlags(imguiWindowFlags)
	{}
	inline LumenWindow(const std::string& name , int imguiWindowFlags = 0)
		: m_windowName(name), m_imguiWindowFlags(imguiWindowFlags)
	{}

	// Destructor.
	inline virtual ~LumenWindow() = default;

private:
	// Set the window ID.
	// If this function is not called the Layer will not work.
	friend class WindowStack;
	void onAttach(unsigned ID);
public:

	// Called by Lumen to render the window.
	void onRender();

	// Removes the window from Lumen.
	void closeWindow();

	// By defult LumenWindows do not need events to be dispatched, since
	// dear imgui handles the events.  Is is provided to be able to pass
	// events to elements contained within the window, for example, engines.
	inline virtual void onEvent(const Event& event) {};

	// Called per frame to update the window and certain events. 
	void onUpdate();

	// ------------------- //
	//  I M G U I   A P I  //
	// ------------------- //

	// ImGui Rendering.  Child classes uses these to render using the imgui API.
	inline virtual void onImGuiBegin() = 0;
	inline virtual void onImGuiRender() = 0;
	inline virtual void onImGuiEnd() = 0;

	// Get the flags related to the ImGuiWindow.
	int getImGuiWindowFlags() const;

	// Sets the imgui flags used when rendering.
	// Overwrites current flags.
	void setImGuiWindowFlags(int flags);

	// Adds the imgui flags used when rendering.
	// Adds to the existing flags.
	void addImGuiWindowFlags(int flags);

	// Removes the imgui flags used when rendering.
	// Removes from the existing flags.
	void removeImGuiWindowFlags(int flags);

	// Checks if the layer is hovered.
	bool isHovered() const;

	// Get the pointer to the imgui window.
	// Curently this has to be called every frame, since the
	// window does not exist on a layer creation.
	ImGuiWindow* findImGuiWindow();

private:
	friend class Application;
	// Focus the window.
	// This function notifies the app of a focus change.
	void focus();
public:

	// Display window with a saved document.
	void unsavedDocument();

	// Display window with an unsaved document.
	void savedDocument();

	// Return the name of the layer as a string ID for imgui.
	const char* getImGuiName() const;

	// Updates the name that imgui uses with appropriate ID's.
	void updateImGuiName();

	// Checks if the Window contents should be rendererd based on certain imgui states.
	bool shouldRender() const;

	// Updates render state flags and check for a change.
	void updateRenderStateFlags();

	// Checks if the render state has changed.
	void detectRenderStateChange();

	// Event handler called when the render state changes.
	// Hepful for when certain intensive resources can be cleared and recreated.
	// (For example, FBO's)
	inline virtual void onRenderStateChange(bool newState) {};

	// Get the available size for rendering.
	const glm::vec2& getContentRegionSize() const;

	// ------------------- //
	//  W I N D O W   I D  //
	// ------------------- //

	// Get the name of the layer.
	const std::string& getName() const;

	// Set & update the window name.
	virtual void setName(const char* name);

	// Get the window's Lumen ID.
	unsigned getID() const;

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Window resize event handler.
	virtual void onWindowResizeEvent(const WindowEvent& event);

	// Window move event handler.
	virtual void onWindowMoveEvent(const WindowEvent& event);

	// Check if window has resized.
	void detectWindowResize();

	// Check if window has moved.
	void detectWindowMove();

	// Convert global mouse cordinates to local coordinates.
	// GLFW window to Lumen window.
	glm::vec2 globalToLocalCoords(const glm::vec2& coords);

	// Convert local mouse cordinates to global coordinates.
	// Lumen window to GLFW window.
	glm::vec2 localToGlobalCoords(const glm::vec2& coords);

	// Return the mouse position in the local scene coordinates (pixels).
	// (0,0) is in the top left.
	glm::vec2 getMouseLocalPosition() const;
	glm::vec2 getMouseGlobalPosition() const;

	// --------- //
	//  D A T A  //
	// --------- //

private:

	int m_imguiWindowFlags = 0;
	bool m_isCollapsed = false;
	bool m_isHidden = false;
	bool m_isDocked = false;
	bool m_skipItems = false;
	bool m_shouldRender = true;
	unsigned m_lumenWindowID = 0;
	std::string m_windowName = "";
	std::string m_imguiName = "";
	ImGuiWindow* m_imguiWindow = nullptr;

protected:

	glm::vec2 m_contentRegionSize = { 0.f, 0.f };
	glm::vec2 m_contentRegionPosition = { 0.f, 0.f };
	bool m_isOpen = true;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//