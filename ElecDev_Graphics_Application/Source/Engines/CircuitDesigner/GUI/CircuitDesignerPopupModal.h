#pragma once

#include "GUI/PopupModal/PopupModal.h"
#include <filesystem>
#include "yaml-cpp/yaml.h"

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

	bool m_deleteComponents = false;
	bool m_deleteCables = false;
	bool m_yamlNode = false;
	YAML::Node m_node;
	int m_entityCount = 0;
	std::string m_entity;

private:

	// Different modals.
	void overWriteModal();
	void deleteModal();
	
};
