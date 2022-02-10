//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "External/ImGUI/Implementations/imgui_impl_glfw.h"
#include "External/ImGUI/Implementations/imgui_impl_opengl3.h"
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
	ImGuiID ribbonDockID = ImGui::DockBuilderSplitNode(m_mainDockspaceID, ImGuiDir_Up, 0.075f, nullptr, &m_mainDockspaceID);
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

	ImGui::ShowDemoWindow();

	// Render all of the layers.
	// The order is not important since dear imgui handles that.
	for (auto& layerPair : m_layerStack->getLayers())
		layerPair.second->onRender();

	// Cleanup.
	onRenderCleanup();
}

void Application::onRenderCleanup()
{
	// Pop custom font.
	ImGui::PopFont();

	// Assign values to viewport for ImGUI.
	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	glm::vec2 viewport(display_w, display_h);
	Renderer::setViewport(viewport);

	// Rendering
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

void Application::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//