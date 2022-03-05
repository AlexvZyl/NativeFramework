//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Application/Application.h"
#include "OpenGL/RendererGL.h"
#include "GLFW/glfw3.h"

//==============================================================================================================================================//
//  Main loop.																																	//
//==============================================================================================================================================//

void Application::run()
{
	// Main loop.
	double waitRemainingTime = 0;
	while (m_isRunning)
	{
		if (m_waitForEvents)
		{
			// Ensure remaining time is positive.
			waitRemainingTime = m_eventsTimeout - m_totalFrameTime;
			if(waitRemainingTime > 0)
				glfwWaitEventsTimeout(waitRemainingTime);
		}
		else glfwPollEvents();

		updateFrametime();

		if (startFrame())
		{
			// Set the frametime 0 at the start of the frame so that the
			// wait for events do not go over the fps.
			m_totalFrameTime = 0;
			onRender();
			updateFrametime();
		}
	}
}

void Application::updateFrametime() 
{
	static double previousTime = 0;
	static double currentTime = 0;
	
	currentTime = glfwGetTime();
	m_totalFrameTime += currentTime - previousTime;
	previousTime = currentTime;
}

bool Application::startFrame() 
{
	return m_totalFrameTime >= m_targetFrameTime;
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

void Application::onRenderInit()
{
	// Clear buffers.
	Renderer::clear();

	{
		LUMEN_PROFILE_SCOPE("ImGui NewFrame");

		// Feed inputs to ImGUI, start new frame.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Enable docking in main viewport.
		// Do we really have to call this every frame?
		m_mainDockspaceID = ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_NoDockingSplitMe);  // NULL uses the main viewport.

		// Push custom font.
		ImGui::PushFont(m_defaultFont);
	}
}

void Application::onRender()
{	
	LUMEN_PROFILE_SCOPE("Frametime");

	// Init.
	onRenderInit();

	// Dispatch the events to the layers before we render them.
	// Has to be called after the init so all of the ImGui data
	// is updated.
	dispatchEvents();

	// Render the layers.
	renderLayers();

	// Cleanup.
	onRenderCleanup();
}

void Application::renderLayers() 
{
	LUMEN_PROFILE_SCOPE("Render Layers");

	// The order is not important since dear imgui handles that.
	for (auto& [name, layer] : m_layerStack->getLayers())
		layer->onRender();
}

void Application::onRenderCleanup()
{
	// Set viewport.
	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	Renderer::setViewport(glm::vec2(display_w, display_h));

	{
		LUMEN_PROFILE_SCOPE("ImGui Draw");

		// Pop custom font.
		ImGui::PopFont();

		// ImGui rendering.
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	{
		LUMEN_PROFILE_SCOPE("Swap Buffers");

		// Swap the window buffers.
		glfwSwapBuffers(m_window);
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
