//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Application.h"
#include "Utilities/Logger/Logger.h"

//==============================================================================================================================================//
//  Dock builder.																																//
//==============================================================================================================================================//

void Application::buildDocks()
{
	// Init.
	onRenderInit();

	// ------------------------- //
	//  D O C K   B U I L D E R  //
	// ------------------------- //

	// Used to build the docks.
	ImGuiID newDockNodeID;
	ImGuiID sceneDockNodeID;

	// Ribbon dock.
	newDockNodeID = ImGui::DockBuilderSplitNode(m_mainDockspacePtr->ID, ImGuiDir_Left, 0.0375f, NULL, &sceneDockNodeID);
	m_ribbonPanelPtr = ImGui::DockBuilderGetNode(newDockNodeID);
	m_ribbonPanelPtr->LocalFlags |= ImGuiDockNodeFlags_NoSplit		| ImGuiDockNodeFlags_NoDockingOverMe
								 | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoResize
								 | ImGuiDockNodeFlags_HiddenTabBar	| ImGuiDockNodeFlags_NoWindowMenuButton
								 | ImGuiDockNodeFlags_NoTabBar;

	// Left Panel.
	newDockNodeID = ImGui::DockBuilderSplitNode(sceneDockNodeID, ImGuiDir_Left, 0.3f, NULL, &sceneDockNodeID);
	m_leftPanelPtr = ImGui::DockBuilderGetNode(newDockNodeID);

	// Bottom Bar.
	newDockNodeID = ImGui::DockBuilderSplitNode(sceneDockNodeID, ImGuiDir_Down, 0.03f, NULL, &sceneDockNodeID);
	m_bottomBarPtr = ImGui::DockBuilderGetNode(newDockNodeID);
	m_bottomBarPtr->LocalFlags |= ImGuiDockNodeFlags_NoSplit		| ImGuiDockNodeFlags_NoDockingOverMe
						 | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoResize
						 | ImGuiDockNodeFlags_HiddenTabBar	| ImGuiDockNodeFlags_NoWindowMenuButton
						 | ImGuiDockNodeFlags_NoTabBar;

	// Right Panel.
	newDockNodeID = ImGui::DockBuilderSplitNode(sceneDockNodeID, ImGuiDir_Right, 0.3f, NULL, &sceneDockNodeID);
	m_rightPanelPtr = ImGui::DockBuilderGetNode(newDockNodeID);
	m_rightPanelPtr->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_AutoHideTabBar;

	// Bottom Panel.
	newDockNodeID = ImGui::DockBuilderSplitNode(sceneDockNodeID, ImGuiDir_Down, 0.3f, NULL, &sceneDockNodeID);
	m_bottomPanelPtr = ImGui::DockBuilderGetNode(newDockNodeID);
	m_bottomPanelPtr->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_AutoHideTabBar;

	// Scene dock.
	m_scenePanelPtr = ImGui::DockBuilderGetNode(sceneDockNodeID);
	m_scenePanelPtr->SharedFlags &= ~ImGuiDockNodeFlags_NoDockingSplitMe;
	m_scenePanelPtr->LocalFlags &= ~ImGuiDockNodeFlags_NoDockingSplitMe;
	m_scenePanelPtr->LocalFlagsInWindows &= ~ImGuiDockNodeFlags_NoDockingSplitMe;

	// Finish dock builder.
	ImGui::DockBuilderFinish(m_mainDockspacePtr->ID);

	// Cleanup.
	onRenderCleanup();
}

//==============================================================================================================================================//
//  Layers																																		//
//==============================================================================================================================================//

void Application::queuePopLayer(Layer* layer)
{
	m_layerStack->queuePopLayer(*layer);
}

void Application::queuePopLayer(std::string& layerName)
{
	Layer* toPop = m_layerStack->getLayer<Layer>(layerName);
	m_layerStack->queuePopLayer(*toPop);
}

void Application::popLayers() 
{
	m_layerStack->popLayers();
}

void Application::dockLayerToPanel(std::string& name, DockPanel panel)
{
	// Dock the layer.
	switch (panel)
	{
	case DockPanel::Scene:
		ImGui::DockBuilderDockWindow(name.c_str(), m_scenePanelPtr->ID);
		break;

	case DockPanel::Left:
		// Could have the left panel only consist of one GUI, like in VS Code.
		ImGui::DockBuilderDockWindow(name.c_str(), m_leftPanelPtr->ID);
		break;

	case DockPanel::Right:
		ImGui::DockBuilderDockWindow(name.c_str(), m_rightPanelPtr->ID);
		break;

	case DockPanel::Bottom:
		ImGui::DockBuilderDockWindow(name.c_str(), m_bottomPanelPtr->ID);
		break;

	case DockPanel::Floating:
		// Do not dock, maybe move to a specific position.
		break;

	case DockPanel::Fixed:
		// These have to be handled manually.
		break;

	case DockPanel::Ribbon:
		ImGui::DockBuilderDockWindow(name.c_str(), m_ribbonPanelPtr->ID);
		break;

	default:
		LUMEN_LOG_WARN("Invalid docking configuration.", "ImGui")
		break;
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
