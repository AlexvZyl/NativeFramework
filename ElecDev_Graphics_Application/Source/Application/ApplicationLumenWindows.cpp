#include "Utilities/Logger/Logger.h"
#include "Application/Application.h"
#include "Application/Windows/LumenWindow.h"
#include "Application/Windows/WindowStack.h"

void Application::queueWindowPop(LumenWindow* window)
{
	m_windowStack->queueWindowPop(window);
}

void Application::queueWindowPop(unsigned ID)
{
	m_windowStack->queueWindowPop(ID);
}

void Application::popWindows() 
{
	m_windowStack->popWindows();
}

std::unordered_map<unsigned, std::unique_ptr<LumenWindow>>& Application::getWindows() 
{
	return m_windowStack->getWindows();
}

LumenWindow* Application::findHoveredWindow()
{
	// We do not have to worry about order, since dear imgui handles it.
	for (auto& [ID, window] : m_windowStack->getWindows())
	{
		if (window->isHovered()) return window.get();
	}
	// No window is hovered.
	return nullptr;
}

LumenWindow* Application::findFocusedWindow()
{
	for (auto& [ID, window] : m_windowStack->getWindows())
	{
		if (window->isFocused()) return window.get();
	}
	// No window is focused.
	return nullptr;
}

void Application::resizeWindows() 
{
	if (!m_resizeWindowsFrameCount) return;

	for (auto& [ID, window] : m_windowStack->getWindows())
	{
		window->detectWindowResize();
	}

	m_resizeWindowsFrameCount--;
}

void Application::dockWindowToPanel(LumenWindow* window, LumenDockPanel panel)
{
	// Dock the Window.
	ImGuiID node = NULL;
	switch (panel)
	{
	case LumenDockPanel::Scene:
		node = findLastActiveChildNode(m_scenePanelID);
		ImGui::DockBuilderDockWindow(window->getImGuiName(), node);
		window->m_dockID = node;
		break;

	case LumenDockPanel::Left:
		// Could have the left panel only consist of one GUI, like in VS Code.
		node = findLastActiveChildNode(m_leftPanelID);
		ImGui::DockBuilderDockWindow(window->getImGuiName(), node);
		window->m_dockID = node;
		break;

	case LumenDockPanel::Right:
		node = findLastActiveChildNode(m_rightPanelID);
		ImGui::DockBuilderDockWindow(window->getImGuiName(), node);
		window->m_dockID = node;
		break;

	case LumenDockPanel::Bottom:
		node = findLastActiveChildNode(m_bottomPanelID);
		ImGui::DockBuilderDockWindow(window->getImGuiName(), node);
		window->m_dockID = node;
		break;

	case LumenDockPanel::Floating:
		// Do not dock, maybe move to a specific position?
		break;

	case LumenDockPanel::Fixed:
		// These have to be handled manually.
		break;

	// These cases should never be docked to more than once, so there is no need
	// to manually update the window ID.

	case LumenDockPanel::Ribbon:
		ImGui::DockBuilderDockWindow(window->getImGuiName(), findLastActiveChildNode(m_ribbonPanelID));
		break;

	case LumenDockPanel::AssetViewer:
		ImGui::DockBuilderDockWindow(window->getImGuiName(), findLastActiveChildNode(m_bottomAssetViewerID));
		break;

	default:
		LUMEN_LOG_WARN("Invalid docking configuration.", "ImGui")
		break;
	}
}

ImGuiID Application::findLargestChildNode(ImGuiID nodeID) 
{
	// Get all of the children.
	std::vector<ImGuiDockNode*> nodes;
	findChildNodes(ImGui::DockBuilderGetNode(nodeID), nodes);

	// If the size is one we only have the parent.
	if (nodes.size() == 1) return nodeID;

	// Find the largest node.
	ImGuiDockNode* largestNode = nodes[0];
	float largestNodeArea = largestNode->Rect().GetArea();
	for (auto node : nodes) 
	{
		float nodeArea = node->Rect().GetArea();
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

	// If the size is one we only have the parent.
	if (nodes.size() == 1) return nodeID;
	
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
	for (auto childNode : currentNode->ChildNodes) 
	{
		// If valid, find its children.
		if (childNode) findChildNodes(childNode, nodes);

		// If any of the two children are NULL it means the current node is at the bottom.
		else 
		{
			nodes.push_back(currentNode);
			return;
		}
	}
}