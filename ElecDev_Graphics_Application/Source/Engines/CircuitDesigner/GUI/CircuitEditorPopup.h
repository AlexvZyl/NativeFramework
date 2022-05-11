#pragma once

#include "Application/LumenWindow/LumenPopupWindow.h"

class CircuitEditorPopup : public LumenPopupWindow
{
public:

	// Constructor.
	CircuitEditorPopup(const std::string& name, int imguiWindowFlags = 0);

	// ImGui rendering.
	virtual void onImGuiRender() override;

	// Set the entity hovered.
	void setCable(const std::string& cable);
	void setComponent(const std::string& component);

private:

	bool m_cable = false;
	bool m_component = false;
	std::string m_entity;
};
