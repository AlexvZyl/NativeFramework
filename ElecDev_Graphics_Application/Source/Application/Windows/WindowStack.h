#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Application/Windows/LumenWindow.h"

//==============================================================================================================================================//
//  Window Stack.																																//
//==============================================================================================================================================//

class WindowStack 
{
public:

	// Constructor.
	inline WindowStack() = default;
	// Destructor.
	inline ~WindowStack() = default;

	// Push a window onto the stack.
	template<class WindowType, class ... Args>
	inline WindowType* pushWindow(const Args& ... args)
	{
		m_windows.insert({m_totalLayerCount, std::make_unique<WindowType>(args...)});
		WindowType* window = getWindow<WindowType>(m_totalLayerCount);
		window->onAttach(m_totalLayerCount++);
		return window;
	}

	// Layers are not removed immediately as this would mean they can 
	// be destroyed while rendering or dispatching events.

	// Queue the window to be removed.
	void queueWindowPop(LumenWindow* window) 
	{
		m_windowPopQueue.push_back(window);
	}

	// Queue the layer to be removed.
	void queueWindowPop(unsigned ID)
	{
		m_windowPopQueue.push_back(getWindow(ID));
	}

	// Pop all the layers that have been queued for removal.
	// We need to pass the layers in use to check if they have been removed.
	void popWindows();

	// Get the windows contained in the stack.
	inline const std::unordered_map<unsigned, std::unique_ptr<LumenWindow>>& getWindows() const
	{
		return m_windows;
	}
	inline std::unordered_map<unsigned, std::unique_ptr<LumenWindow>>& getWindows()
	{
		return m_windows;
	}

	// Clear the stack.
	inline void clear() 
	{
		m_windowPopQueue.clear();
		m_windows.clear();
	}

 	// Retrieve the layer from the stack with an attempted cast to the requested type.
	template<class WindowType>
	WindowType* getWindow(unsigned ID) const
	{
		return dynamic_cast<WindowType*>(m_windows.at(ID).get());
	}

	// Retrieve the window from the stack as a base pointer.
	LumenWindow* getWindow(unsigned ID) const
	{
		return m_windows.at(ID).get();
	}

private:

	// Map containing all of the windows.  The key is the window ID.
	std::unordered_map<unsigned, std::unique_ptr<LumenWindow>> m_windows;

	// Windows that are queued for removal.
	std::vector<LumenWindow*> m_windowPopQueue;

	// Count of the total layers that have passed through the stack.
	// Used to create unique ID's for the layers.
	unsigned m_totalLayerCount = 0;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
