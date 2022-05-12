#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <memory>
#include "Application/LumenWindow/LumenWindow.h"
#include "Engines/EngineCore/EngineCore.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "GUI/LumenPayload/LumenPayload.h"

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
		addImGuiWindowFlags(ImGuiWindowFlags_NoScrollWithMouse);
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
		ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
	}

	inline virtual void onImGuiRender() override
	{
		// Render design palette.
		if (m_engine->hasDesignPalette())
		{
			// Set flag.
			addImGuiWindowFlags(ImGuiWindowFlags_MenuBar);

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
		// Set flag for no palette.
		else removeImGuiWindowFlags(ImGuiWindowFlags_MenuBar);

		// Render engine scene.
		m_engine->onRender();
		if (!m_textureID) return;
		ImGui::Image(m_textureID, m_contentRegionSize, ImVec2(0, 1), ImVec2(1, 0));
		// Check if image is hovered to allow blocking of events.
		if (ImGui::IsItemHovered()) m_engine->m_isHovered = true;
		else						m_engine->m_isHovered = false;

		// Drag & Drop files.
		LumenPayload payloadFile(LumenPayloadType::String);
		payloadFile.setDragAndDropTarget();
		if (payloadFile.hasValidData()) m_engine->onEvent(FileDropEvent(payloadFile.getDataString(), EventType_FileDrop));

		// Drag & Drop nodes.
		LumenPayload payloadNode(LumenPayloadType::YamlNode);
		payloadNode.setDragAndDropTarget();
		if (payloadNode.hasValidData()) m_engine->onEvent(YamlNodeDropEvent(payloadNode.getDataYamlNode()));
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

		// LumenWindows receive events in global window coordinates.  These have to be converted to local
		// coordinates before being passed to the engine.

		// Mouse events.
		if (event.isType(EventType_MouseMove))
		{
			const MouseMoveEvent& mouseEvent = event.cast<MouseMoveEvent>();
			m_engine->onEvent(MouseMoveEvent(
				globalToLocalCoords(mouseEvent.mousePosition), 
				mouseEvent.ID
			));
		}
		else if (event.isType(EventType_MouseDrag))
		{
			const MouseDragEvent& mouseEvent = event.cast<MouseDragEvent>();
			m_engine->onEvent(MouseDragEvent(
				globalToLocalCoords(mouseEvent.initialPosition), 
				globalToLocalCoords(mouseEvent.mousePosition),
				mouseEvent.currentFrameDelta,
				mouseEvent.ID
			));
		}
		else if (event.isType(EventType_MouseScroll))
		{
			const MouseScrollEvent& mouseEvent = event.cast<MouseScrollEvent>();
			m_engine->onEvent(MouseScrollEvent(
				globalToLocalCoords(mouseEvent.mousePosition),
				mouseEvent.yOffset,
				mouseEvent.xOffset,
				mouseEvent.ID
			));
		}
		else if (event.isType(EventType_MousePress) || event.isType(EventType_MouseRelease || event.isType(EventType_MouseDoublePress)))
		{
			const MouseButtonEvent& mouseEvent = event.cast<MouseButtonEvent>();
			m_engine->onEvent(MouseButtonEvent(
				globalToLocalCoords(mouseEvent.mousePosition),
				mouseEvent.ID
			));
		}

		// Key events.
		else if (event.isType(EventType_KeyPress) || event.isType(EventType_KeyRelease) || event.isType(EventType_KeyRepeat))
		{
			const KeyEvent& keyEvent = event.cast<KeyEvent>();
			m_engine->onEvent(KeyEvent(
				keyEvent.key,
				keyEvent.ID,
				globalToLocalCoords(keyEvent.mousePosition)
			));
		}

		// The other events do not need adjustments, since they do no contain mouse positions.
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
	// This allows Lumen to work with engines seperately from the GUI.
	template<class ... Args>
	inline void constructEngine(const Args& ... args)
	{
		m_engine = std::make_unique<EngineType>(args...);
		m_textureID = (void*)m_engine->getRenderTexture();
		// Scene is bound in EngineCore.
		// If the scene has to be bound to be rendered to it will happen on the focus.
		Renderer::restoreAndUnbindScene();  
		m_engine->m_parentWindow = this;
		// Update the engine name.
		m_engine->setName(getName());
	}

	// Pass the resize to the engine.
	inline void onWindowResizeEvent(const WindowEvent& event) override 
	{
		LumenWindow::onWindowResizeEvent(event);
		m_engine->onWindowResizeEventForce(event);
	}

	// Override to take engine hoevered into account.
	// This allows ImGui widgets to block events to the engine.
	inline virtual bool isHovered() const override 
	{
		return LumenWindow::isHovered() && m_engine->m_isHovered;
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