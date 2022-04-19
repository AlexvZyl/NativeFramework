//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Profiler/Profiler.h"
#include "imgui/notify/imgui_notify.h"
#include "Application/Application.h"
#include "Application/LumenWindow/LumenWindow.h"
#include "Application/LumenWindow/WindowStack.h"
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
			renderFrame();
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
	Renderer::clearColor();

	{
		LUMEN_PROFILE_SCOPE("ImGui NewFrame");

		// Feed inputs to ImGUI, start new frame.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Enable docking in main viewport.
		m_mainDockspaceID = ImGui::DockSpaceOverViewport(NULL);//, ImGuiDockNodeFlags_NoDockingSplitMe);  // NULL uses the main viewport.

		// Push custom font.
		ImGui::PushFont(m_defaultFont);
	}
}

void Application::renderFrame()
{
	{
		LUMEN_PROFILE_SCOPE("Frametime (App)");

		{
			LUMEN_PROFILE_SCOPE("Frametime (CPU)");

			// Update imgui states before starting new frame.
			// These states are controlled by Lumen for optimistaion.
			imguiOnUpdate();

			// Called before onUpdate so that ImGui::NewFrame() can be called
			// before LumenWindow events are dispatched.
			onRenderInit();

			// Updates Lumen state and LumenWindows.
			onUpdate();

			// ImGui calls and engine rendering.
			onRender();

			// Cleanup & imgui drawing.
			onRenderCleanup();
		}
		{
			// Keep this seperate, since it blocks.
			// This must be added to the Frametime (GPU) when added.
			LUMEN_PROFILE_SCOPE("Swap Buffers");
			glfwSwapBuffers(getGLFWWindow());
		}
	}
}

void Application::onRender()
{
	LUMEN_PROFILE_SCOPE("App OnRender");

	// The order is not important, since dear imgui handles that.
	for (auto& [ID, window] : m_windowStack->getWindows())
		window->onRender();

	// Push notificatons.
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8.f, 8.f});
	ImGui::RenderNotifications();
	ImGui::PopStyleVar();
}

void Application::onRenderCleanup()
{
	// Set viewport.
	int display_w, display_h;
	glfwGetFramebufferSize(getGLFWWindow(), &display_w, &display_h);
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
}

//==============================================================================================================================================//
//  Notifications.																																//
//==============================================================================================================================================//

void Application::pushNotification(NotificationType type, int msTime, const std::string& content, const std::string& title)
{
	// Get the type.
	ImGuiToastType notificationType = 0;
	switch (type)
	{
	case NotificationType::Success:
		notificationType = ImGuiToastType_Success;
		break;
	case NotificationType::Warning:
		notificationType = ImGuiToastType_Warning;
		break;
	case NotificationType::Info:
		notificationType = ImGuiToastType_Info;
		break;
	case NotificationType::Error:
		notificationType = ImGuiToastType_Error;
		break;
	}

	ImGuiToast notification(notificationType, msTime);
	notification.set_content(content.c_str());
	if (title.size())
		notification.set_title(title.c_str());

	ImGui::InsertNotification(notification);
}

//==============================================================================================================================================//
//  Dock builder.																																//
//==============================================================================================================================================//

void Application::buildDocks()
{
	// Init.
	onRenderInit();

	// Pointer used to access nodes.
	ImGuiDockNode* dockNode = nullptr;

	// Ribbon dock.
	m_ribbonPanelID = ImGui::DockBuilderSplitNode(m_mainDockspaceID, ImGuiDir_Left, 0.0375f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_ribbonPanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoDockingOverMe
		| ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoResize
		| ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton
		| ImGuiDockNodeFlags_NoTabBar;

	// Left Panel.
	m_leftPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Left, 0.3f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_leftPanelID);

	// Bottom Bar.
	m_bottomBarID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Down, 0.03f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_bottomBarID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoDockingOverMe
		| ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoResize
		| ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton
		| ImGuiDockNodeFlags_NoTabBar;

	// Right Panel.
	m_rightPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Right, 0.3f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_rightPanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_AutoHideTabBar;

	// Bottom Panel.
	m_bottomPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Down, 0.3f, NULL, &m_scenePanelID);

	// Bottom Asset viewer.
	m_bottomAssetViewerID = ImGui::DockBuilderSplitNode(m_bottomPanelID, ImGuiDir_Right, 0.2f, NULL, &m_bottomPanelID);
	dockNode = ImGui::DockBuilderGetNode(m_bottomAssetViewerID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar 
						 |  ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingSplitMe;
	dockNode = ImGui::DockBuilderGetNode(m_bottomPanelID);
	dockNode->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_AutoHideTabBar;

	// Scene dock.
	dockNode = ImGui::DockBuilderGetNode(m_scenePanelID);

	// Finish dock builder.
	ImGui::DockBuilderFinish(m_mainDockspaceID);

	// Cleanup.
	onRenderCleanup();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//