#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include "Application/LumenWindow/LumenWindow.h"
#include "Engines/EngineCore/EngineCore.h"
#include "OpenGL/SceneGL.h"

//==============================================================================================================================================//
//  Graphics Scene.																																//
//==============================================================================================================================================//

template<class EngineType>
class GraphicsScene : public LumenWindow
{
public:

	// Constructor.
	inline GraphicsScene(const std::string& name, int windowFlags = 0) 
		: LumenWindow(name, windowFlags | ImGuiWindowFlags_NoScrollbar)
	{
		m_imguiWindowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	}

	// Destructor.
	inline virtual ~GraphicsScene() 
	{
		Renderer::initSceneDestruction(m_engine->m_scene.get());
	}

	inline virtual void onImGuiBegin() override 
	{	
		// Adjust window padding.
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin(getImGuiName(), &m_isOpen, m_imguiWindowFlags);
	}

	inline virtual void onImGuiRender() override 
	{
		// Set flag for design palette.
		if (m_engine->hasDesignPalette())  { m_imguiWindowFlags |= ImGuiWindowFlags_MenuBar;  }
		else							   { m_imguiWindowFlags &= ~ImGuiWindowFlags_MenuBar; }

		// Render design palette.
		if (m_engine->hasDesignPalette())
		{
			// Setup style.
			ImGui::PopStyleVar();
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
			ImGui::PushID(getName().c_str());
			// Render palette.
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);
				m_engine->renderDesignPalette();
				ImGui::EndMenuBar();
				ImGui::PopStyleVar();
			}
			// Clear style.
			ImGui::PopID();
			ImGui::PopStyleColor();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		}

		// Render engine scene.
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 1.f);
		m_engine->onRender();
		if (!m_textureID) return;
		ImGui::Image(m_textureID, { m_contentRegionSize.x + 1.f, m_contentRegionSize.y + 1.f }, ImVec2(0, 1), ImVec2(1, 0));

		// Receive dropped files.
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				// Pass FileDropEvent to engine.
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::wstring wPath(path);
				std::string sPath;
				sPath.insert(sPath.end(), wPath.begin(), wPath.end());
				m_engine->onEvent(FileDropEvent(sPath, EventType_FileDrop));
			}
			ImGui::EndDragDropTarget();
		}
	}

	inline virtual void onImGuiEnd() override 
	{
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}

	// Pass events to the engine.
	inline virtual void onEvent(const Event& event) override 
	{
		if (event.isConsumed()) return;

		// LumenWindow receives events in global window coordinates.  These have to be converted to local
		// coordinates before being passed to the engine.

		// Mouse events.
		if (event.isType(EventType_MouseMove))
		{
			const MouseMoveEvent& mouseEvent = dynamic_cast<const MouseMoveEvent&>(event);
			m_engine->onEvent(MouseMoveEvent(
				globalToLocalCoords(mouseEvent.mousePosition), 
				mouseEvent.ID
			));
		}
		else if (event.isType(EventType_MouseDrag))
		{
			const MouseDragEvent& mouseEvent = dynamic_cast<const MouseDragEvent&>(event);
			m_engine->onEvent(MouseDragEvent(
				globalToLocalCoords(mouseEvent.initialPosition), 
				globalToLocalCoords(mouseEvent.mousePosition),
				mouseEvent.currentFrameDelta,
				mouseEvent.ID
			));
		}
		else if (event.isType(EventType_MouseScroll))
		{
			const MouseScrollEvent& mouseEvent = dynamic_cast<const MouseScrollEvent&>(event);
			m_engine->onEvent(MouseScrollEvent(
				globalToLocalCoords(mouseEvent.mousePosition),
				mouseEvent.yOffset,
				mouseEvent.xOffset,
				mouseEvent.ID
			));
		}
		else if (event.isType(EventType_MousePress) || event.isType(EventType_MouseRelease || event.isType(EventType_MouseDoublePress)))
		{
			const MouseButtonEvent& mouseEvent = dynamic_cast<const MouseButtonEvent&>(event);
			m_engine->onEvent(MouseButtonEvent(
				globalToLocalCoords(mouseEvent.mousePosition),
				mouseEvent.ID
			));
		}

		// Key events.
		else if (event.isType(EventType_KeyPress) || event.isType(EventType_KeyRelease) || event.isType(EventType_KeyRepeat))
		{
			const KeyEvent& keyEvent = dynamic_cast<const KeyEvent&>(event);
			m_engine->onEvent(KeyEvent(
				keyEvent.key,
				keyEvent.ID,
				globalToLocalCoords(keyEvent.mousePosition)
			));
		}

		// The other events do not need adjustments.
		else m_engine->onEvent(event);
	}

	// Event handler for when the render state changes.
	inline virtual void onRenderStateChange(bool newState) override 
	{
		// Create FBO resources.
		if (newState)
		{
			m_engine->m_scene->recreateGPUResources();
			m_textureID = (void*)m_engine->getRenderTexture();
		}

		// Destroy FBO resources.
		else
		{
			m_engine->m_scene->deleteGPUResources();
			m_textureID = nullptr;
		}
	}

	// Get the engine inside the graphics scene.
	inline EngineType* getEngine() 
	{
		return m_engine.get();
	}

	// Seperate method to construct the engine.
	// This allows Lumen to work with engines seperately from GUI'.
	template<class ... Args>
	inline void constructEngine(const Args& ... args)
	{
		m_engine = std::make_unique<EngineType>(args...);
		m_textureID = (void*)m_engine->getRenderTexture();
	}

	// Pass the resize to the engine.
	inline void onWindowResizeEvent(const WindowEvent& event) override 
	{
		LumenWindow::onWindowResizeEvent(event);
		m_engine->onWindowResizeEventForce(event);
	}

private:

	// Rendering texture.
	void* m_textureID = nullptr;
	// The engine that is rendered in the window.
	std::unique_ptr<EngineType> m_engine = nullptr;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
