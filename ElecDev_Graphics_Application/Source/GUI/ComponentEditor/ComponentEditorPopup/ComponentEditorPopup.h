#pragma once
#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Design2DEngine;

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class ComponentEditorPopup : public GuiElementCore
{
public:

	// Constructor.
	ComponentEditorPopup(std::string name, int imguiWindowFlags);
	// Destructor.
	inline virtual ~ComponentEditorPopup() = default;

	// Set the initial position.
	void setPosition(const glm::vec2& pos);

	// Set the engine that the popup is in.
	void setComponentEditor(ComponentEditor* editor);

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

private:

	// The component editor this GUI belongs to.
	ComponentEditor* m_compEditor = nullptr;

	// The position the popup has to be dispayed.
	glm::vec2 m_pos = { 0,0 };

	// We need to close the popup on a defocus.
	virtual void onDefocusEvent(LayerEvent& event) override;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//