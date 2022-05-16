#pragma once

// Includes.
#include "Application/LumenWindow/LumenWindow.h"

class PopupModal : public LumenWindow
{
public:

	// Constructor.
	PopupModal(const std::string& name, int imguiWindowFlags = 0)
		: LumenWindow(name, imguiWindowFlags)
	{
		addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	}

	// Rendering.
	inline virtual void onImGuiBegin() final
	{
		ImGui::OpenPopup(getImGuiName(), getImGuiWindowFlags());
		m_isModalOpen = ImGui::BeginPopupModal(getImGuiName(), NULL, getImGuiWindowFlags());
	}

	virtual void onImGuiRender() override = 0;

	inline virtual void onImGuiEnd() final 
	{
		if (m_isModalOpen) ImGui::EndPopup();
	}

	// Close the modal window.
	inline virtual void closeWindow() 
	{
		ImGui::CloseCurrentPopup();
		LumenWindow::closeWindow();
	}

private:

	// Is the modal open?
	bool m_isModalOpen = true;
};
