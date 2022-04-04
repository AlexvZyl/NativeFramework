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
	//dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;

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
	//dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_AutoHideTabBar;

	// Bottom Panel.
	m_bottomPanelID = ImGui::DockBuilderSplitNode(m_scenePanelID, ImGuiDir_Down, 0.3f, NULL, &m_scenePanelID);
	dockNode = ImGui::DockBuilderGetNode(m_bottomPanelID);
	//dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_AutoHideTabBar;

	// Scene dock.
	dockNode = ImGui::DockBuilderGetNode(m_scenePanelID);
	//dockNode->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplitMe;

	// Finish dock builder.
	ImGui::DockBuilderFinish(m_mainDockspaceID);

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
		ImGui::DockBuilderDockWindow(name.c_str(), findLastActiveChildNode(m_scenePanelID));
		break;

	case DockPanel::Left:
		// Could have the left panel only consist of one GUI, like in VS Code.
		ImGui::DockBuilderDockWindow(name.c_str(), m_leftPanelID);
		break;

	case DockPanel::Right:
		ImGui::DockBuilderDockWindow(name.c_str(), m_rightPanelID);
		break;

	case DockPanel::Bottom:
		ImGui::DockBuilderDockWindow(name.c_str(), m_bottomPanelID);
		break;

	case DockPanel::Floating:
		// Do not dock, maybe move to a specific position.
		break;

	case DockPanel::Fixed:
		// These have to be handled manually.
		break;

	case DockPanel::Ribbon:
		ImGui::DockBuilderDockWindow(name.c_str(), m_ribbonPanelID);
		break;

	default:
		LUMEN_LOG_WARN("Invalid docking configuration.", "IMGUI")
			break;
	}
}

//==============================================================================================================================================//
//  Docking nodes.																																//
//==============================================================================================================================================//

ImGuiID Application::findLargestChildNode(ImGuiID nodeID) 
{
	// Get all of the children.
	std::vector<ImGuiDockNode*> nodes;
	findChildNodes(ImGui::DockBuilderGetNode(nodeID), nodes);

	// No children found.
	if (!nodes.size())
		return nodeID;

	// Find the largest node.
	ImGuiDockNode* largestNode = nodes[0];
	float largestNodeArea = largestNode->Size.x * largestNode->Size.y;
	for (auto* node : nodes) 
	{
		float nodeArea = node->Size.x * node->Size.y;
		if (nodeArea > largestNodeArea)
		{
			largestNode = node;
			largestNodeArea = nodeArea;
		}
	}
	return largestNode->ID;
}

ImGuiID Application::findLastActiveChildNode(ImGuiID nodeID)
{
	// Get all of the children.
	std::vector<ImGuiDockNode*> nodes;
	findChildNodes(ImGui::DockBuilderGetNode(nodeID), nodes);

	// No children found.
	if (!nodes.size())
		return nodeID;

	// Find the last active node.
	ImGuiDockNode* latestNode = nodes[0];
	for (auto* node : nodes)
	{
		if (node->LastFrameFocused > latestNode->LastFrameFocused)
		{
			latestNode = node;
		}
	}
	return latestNode->ID;
}

void Application::findChildNodes(ImGuiDockNode* currentNode, std::vector<ImGuiDockNode*>& nodes)
{
	// Iterate over children.
	for (auto* childNode : currentNode->ChildNodes) 
	{
		if (childNode)
		{
			// If valid, find its children.
			findChildNodes(childNode, nodes);
			// Add valid child to vector.
			nodes.push_back(childNode);
		}
	}
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
