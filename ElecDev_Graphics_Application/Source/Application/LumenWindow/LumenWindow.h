#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>
#include "Application/Application.h"
#include "Lumen.h"
#include "Application/Events/Events.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Event;

//==============================================================================================================================================//
//  Layer Base Class.																															//
//==============================================================================================================================================//

// LumenWindow is a wrapper class around a dear imgui window so that 
// Lumen can work with it in an OOP fashion.
class LumenWindow
{
public:

	// Constructor.
	// IMPORTANT: onAttach() has to be called when the layer is pushed onto the stack.
	inline LumenWindow(const char* name, int imguiWindowFlags)
		: m_windowName(name), m_imguiWindowFlags(imguiWindowFlags)
	{}
	inline LumenWindow(const std::string& name , int imguiWindowFlags)
		: m_windowName(name), m_imguiWindowFlags(imguiWindowFlags)
	{}

	// Destructor.
	inline virtual ~LumenWindow() = default;

private:
	// Set the window ID.
	// If this function is not called the Layer will not work.
	friend class WindowStack;
	inline void onAttach(unsigned ID)
	{
		m_lumenWindowID = ID;
		updateImGuiName();
	}
public:

	// Called by Lumen to render the window.
	inline void onRender() 
	{
		onImGuiBegin();

		if (shouldRender())
			onImGuiRender();

		onImGuiEnd();

		if (!m_isOpen)
			closeWindow();
	}

	// Removes the window from Lumen.
	inline virtual void closeWindow() 
	{
		Lumen::getApp().queueWindowPop(this);
	}

	// By defult LumenWindows do not need events to be dispatched, since
	// dear imgui handles the events.  Is is provided to be able to pass
	// events to elements contained within the window, for example, engines.
	inline virtual void onEvent(const Event& event) {};

	// Called per frame to update the window and certain events. 
	inline void onUpdate() 
	{
		// If no window is found it should not be updated.
		if (!findImGuiWindow()) return;

		updateRenderStateFlags();

		// Do not check for events if it is not being rendererd.
		if (!shouldRender()) return;

		detectWindowMove();
		detectWindowResize();
	}

	// ------------------- //
	//  I M G U I   A P I  //
	// ------------------- //

	// ImGui Rendering.  Child classes uses these to render using the imgui API.
	inline virtual void onImGuiBegin() = 0;
	inline virtual void onImGuiRender() = 0;
	inline virtual void onImGuiEnd() = 0;

	// Checks if the layer is hovered.
	inline bool isHovered() const
	{
		if (!m_imguiWindow) return false; // No window currently exists.
		return ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_DockHierarchy, m_imguiWindow);
	}

	// Get the pointer to the imgui window.
	// Curently this has to be called every frame, since the
	// window does not exist on a layer creation.
	inline ImGuiWindow* findImGuiWindow() 
	{
		if (m_imguiWindow) return m_imguiWindow;  // Window already found.
		m_imguiWindow = ImGui::FindWindowByName(getImGuiName());
		return m_imguiWindow;
	}

	// Focus the window.
	inline void focus() const
	{
		ImGui::SetWindowFocus(getImGuiName());
	}

	// Display window with a saved document.
	inline void unsavedDocument()
	{
		m_imguiWindowFlags |= ImGuiWindowFlags_UnsavedDocument;
	}

	// Display window with an unsaved document.
	inline void savedDocument()
	{
		m_imguiWindowFlags &= ~ImGuiWindowFlags_UnsavedDocument;
	}

	// Return the name of the layer as a string ID for imgui.
	inline const char* getImGuiName() const
	{
		return m_imguiName.c_str();
	}

	// Checks if the Window contents should be rendererd based on certain imgui states.
	inline bool shouldRender() const
	{
		return m_shouldRender;
	}

	// Updates render state flags and check for a change.
	inline void updateRenderStateFlags() 
	{
		m_isCollapsed = m_imguiWindow->Collapsed;
		m_isDocked = m_imguiWindow->DockIsActive;
		m_isHidden = m_imguiWindow->Hidden;
		detectRenderStateChange();
	}

	// Checks if the render state has changed.
	inline void detectRenderStateChange() 
	{
		// Check for shouldRender state change.
		bool newState = !m_isCollapsed && !m_isHidden && m_isOpen;
		if (m_shouldRender != newState)
		{
			onRenderStateChange(newState);
			m_shouldRender = newState;
		}
	}

	// Event handler called when the render state changes.
	// Hepful for when certain intensive resources can be cleared and recreated.
	// (For example, FBO's)
	inline virtual void onRenderStateChange(bool newState) {};

	// ------------------- //
	//  W I N D O W   I D  //
	// ------------------- //

	// Get the name of the layer.
	inline const std::string& getName() const
	{
		return m_windowName;
	}

	// Set & update the window name.
	inline virtual void setName(const char* name)
	{
		m_windowName = name;
		updateImGuiName();
	}

	// Updates the name that imgui uses with appropriate ID's.
	inline void updateImGuiName() 
	{
		m_imguiName = m_windowName + "###LumenLayer" + std::to_string(m_lumenWindowID);
	}

	// Get the window's Lumen ID.
	inline unsigned getID() const
	{
		return m_lumenWindowID;
	}

	// ------------------- //
	//  U T I L I T I E S  //
	// ------------------- //

	// Window resize event handler.
	inline virtual void onWindowResizeEvent(const WindowEvent& event) 
	{
		m_contentRegionSize = { event.windowData.x, event.windowData.y };
	}

	// Window move event handler.
	inline virtual void onWindowMoveEvent(const WindowEvent& event) 
	{
		m_contentRegionPosition = { event.windowData.x, event.windowData.y };
	}

	// Check if window has resized.
	inline void detectWindowResize()
	{
		glm::vec2 contentRegionSize = m_imguiWindow->WorkRect.GetSize();
		if (m_contentRegionSize.x != contentRegionSize.x || m_contentRegionSize.y != contentRegionSize.y)
		{
			onWindowResizeEvent(WindowEvent(contentRegionSize, EventType_WindowResize));
		}
	}

	// Check if window has moved.
	inline void detectWindowMove() 
	{
		glm::vec2 contentRegionPos = m_imguiWindow->WorkRect.Min;
		if (m_contentRegionPosition.x != contentRegionPos.x || m_contentRegionPosition.y != contentRegionPos.y)
		{
			onWindowMoveEvent(WindowEvent(contentRegionPos, EventType_WindowMove));
		}
	}

	// Return the mouse position in the local scene coordinates (pixels).
	// (0,0) is in the top left.
	inline glm::vec2 getMouseLocalPosition() const
	{
		double cursorX, cursorY;
		glfwGetCursorPos(Lumen::getApp().getGLFWWindow(), &cursorX, &cursorY);
		return { cursorX - m_contentRegionPosition.x, cursorY - m_contentRegionPosition.y };
	}

	inline glm::vec2 getMouseGlobalPosition() const
	{
		double cursorX, cursorY;
		glfwGetCursorPos(Lumen::getApp().getGLFWWindow(), &cursorX, &cursorY);
		return { cursorX, cursorY };
	}

	// --------- //
	//  D A T A  //
	// --------- //

private:

	bool m_isCollapsed = false;
	bool m_isHidden = false;
	bool m_isDocked = false;
	bool m_shouldRender = true;
	std::string m_windowName = "";
	std::string m_imguiName = "";
	unsigned m_lumenWindowID = 0;
	ImGuiWindow* m_imguiWindow = nullptr;

protected:

	glm::vec2 m_contentRegionSize = { 0.f, 0.f };
	glm::vec2 m_contentRegionPosition = { 0.f, 0.f };
	bool m_isOpen = true;
	int m_imguiWindowFlags = 0;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//