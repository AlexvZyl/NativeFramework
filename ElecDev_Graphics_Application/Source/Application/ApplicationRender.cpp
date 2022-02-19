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
//  Rendering.																																	//
//==============================================================================================================================================//

void Application::renderInitialFrame() 
{	
	// Init.
	onRenderInit();

	// Main ribbon dock space.
	ImGuiID ribbonDockID = ImGui::DockBuilderSplitNode(m_mainDockspaceID, ImGuiDir_Left, 0.035f, nullptr, &m_mainDockspaceID);
	ImGuiDockNode* ribbonDockNode = ImGui::DockBuilderGetNode(ribbonDockID);
	// FIX THIS!
	ribbonDockNode->LocalFlags	|= ImGuiDockNodeFlags_NoTabBar			| ImGuiDockNodeFlags_NoDockingInCentralNode	
								|  ImGuiDockNodeFlags_NoCloseButton		| ImGuiDockNodeFlags_NoDocking
								|  ImGuiDockNodeFlags_NoDockingSplitMe	| ImGuiDockNodeFlags_NoDockingOverMe
								| ImGuiDockNodeFlags_DockSpace;							
	ImGui::DockBuilderDockWindow("Main Ribbon##1", ribbonDockID);  // Only valid if main ribbon added second.
	ImGui::DockBuilderFinish(m_mainDockspaceID);

	// Side panels.


	// Cleanup.
	onRenderCleanup();
}

void Application::onRenderInit()
{
	// Clear buffers.
	Renderer::clear();

	// Feed inputs to ImGUI, start new frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Enable main viewport docking.
	m_mainDockspaceID = ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);  // NULL uses the main viewport.

	// Push custom font.
	ImGui::PushFont(m_defaultFont);
}

void Application::onRender()
{	
	// Init.
	onRenderInit();

	// Dispatch the events to the layers before we render them.
	// Has to be called after the init so all of the ImGui data
	// is updated.
	dispatchEvents();

	// Render all of the layers.
	// The order is not important since dear imgui handles that.
	for (auto& [name, layer] : m_layerStack->getLayers())
		layer->onRender();

	// Some layers are queued to pop after rendering.
	m_layerStack->popLayers();

	// Cleanup.
	onRenderCleanup();
}

void Application::onRenderCleanup()
{
	// Pop custom font.
	ImGui::PopFont();

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	Renderer::setViewport(glm::vec2(display_w, display_h));
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

	// Push commands to the GPU.
	Renderer::finish();

	// Swap the window buffers.
	glfwSwapBuffers(m_window);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
