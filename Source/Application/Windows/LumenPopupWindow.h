#pragma once

#include "Application/Windows/LumenWindow.h"

class NotifyEvent;
class Event;

class LumenPopupWindow : public LumenWindow
{
public: 

	// Constructor.
	LumenPopupWindow(const std::string& name, int imguiWindowFlags = 0);

	// Imgui rendering.
	virtual void onImGuiBegin() override;
	inline virtual void onImGuiRender() override = 0;
	virtual void onImGuiEnd() override;

	// Events.
	virtual void onEvent(const Event& event) override;

	// Set the position of the Popup in global coordinates.
	void setPosition(const glm::vec2& position);

private:

	// Data.
	glm::vec2 m_position = { 0.f, 0.f };

	// Events.
	virtual void onDefocusEvent(const NotifyEvent& event);
};