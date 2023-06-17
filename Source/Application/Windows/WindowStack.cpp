//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Windows/LumenWindow.h"
#include "Application/Windows/WindowStack.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  window management.																															//
//==============================================================================================================================================/

void WindowStack::popWindows()
{
	// Check if there are windows to remove.
	if (!m_windowPopQueue.size()) return;

	Application& app = Lumen::getApp();

	// Get active windows.
	LumenWindow* hoveredWindow = app.m_hoveredWindow;
	LumenWindow* focusedWindow = app.m_focusedWindow;

	// Remove windows in queue.
	for (LumenWindow* window : m_windowPopQueue)
	{
		// Check for active window change.
		if (window == hoveredWindow) app.m_hoveredWindow = nullptr;
		if (window == focusedWindow)
		{
			app.m_focusedWindow = nullptr;
			ImGui::SetWindowFocus(NULL);
		}
		// Remove window.
		m_windows.erase(window->getID());
	}

	// All the windows have been removed.
	m_windowPopQueue.clear();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//