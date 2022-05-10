#pragma once

#include "GUI/PopupModal/PopupModal.h"

class CircuitDesignerPopupModal : public PopupModal
{
public:

	// Constructor.
	CircuitDesignerPopupModal(const std::string& name, int imguiWindowFlags = 0);
	// Destructor.
	inline ~CircuitDesignerPopupModal() = default;

	// Render.
	virtual void onImGuiRender() override;

	// Path of the entity that is about to be overwritten.
	std::filesystem::path m_entityPath;
	bool m_cableOverwrite = false;
	bool m_componentOverwrite = false;

	// Store the mouse position when the modal was opened.
	glm::vec2 m_mousePosition = {-1.f,-1.f};
};
