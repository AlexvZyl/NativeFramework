#pragma once

#include "Application/Application.h"
#include "Lumen.h"
#include "Application/LumenWindow/LumenWindow.h"
#include "Application/LumenWindow/WindowStack.h"
#include "GUI/GraphicsScene/GraphicsScene.h"

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