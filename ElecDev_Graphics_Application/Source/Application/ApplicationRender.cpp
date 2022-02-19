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

	// Pointer used to access nodes.
	ImGuiDockNode* dockNode = nullptr;

	// Ribbon dock.
	ImGuiID ribbonDockID = ImGui::DockBuilderSplitNode(m_mainDockspaceID, ImGuiDir_Left, 0.033f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(ribbonDockID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoSplit		| ImGuiDockNodeFlags_NoDockingOverMe
						 | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoResize
						 | ImGuiDockNodeFlags_HiddenTabBar	| ImGuiDockNodeFlags_NoWindowMenuButton
						 | ImGuiDockNodeFlags_NoTabBar;
	ImGui::DockBuilderDockWindow("Main Ribbon##1", ribbonDockID);  // Only valid if main ribbon added second.
	ImGui::DockBuilderFinish(m_mainDockspaceID);

	// Left Panel.
	m_leftPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Left, 0.1f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_leftPanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;

	// Right Panel.
	m_rightPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Right, 0.1f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_rightPanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;

	// Bottom Panel.
	m_bottomPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Down, 0.15f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_bottomPanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;

	// Scene dock.
	dockNode = ImGui::DockBuilderGetNode(m_scenePanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;

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
