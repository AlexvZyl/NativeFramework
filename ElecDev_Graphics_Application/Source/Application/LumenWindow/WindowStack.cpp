//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"
#include "Application/LumenWindow/WindowStack.h"
#include "Lumen.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  Layer management.																															//
//==============================================================================================================================================/

void WindowStack::popWindows()
{
	// Check if there are layers to remove.
	if (!m_windowPopQueue.size()) return;

	Application& app = Lumen::getApp();

	// Get active layers.
	LumenWindow* hoveredWindow = app.m_hoveredWindow;
	LumenWindow* focusedWindow = app.m_focusedWindow;

	// Remove layers in queue.
	for (LumenWindow* window : m_windowPopQueue)
	{
		// Check for active layer change.
		if (window == hoveredWindow) app.m_hoveredWindow = nullptr;
		if (window == focusedWindow)
		{
			// Reset.
			app.m_focusedWindow = nullptr;
			ImGui::SetWindowFocus(NULL);
		}
		// Remove layer.
		m_windows.erase(window->getID());
	}

	// All the layers have been removed.
	m_windowPopQueue.clear();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//