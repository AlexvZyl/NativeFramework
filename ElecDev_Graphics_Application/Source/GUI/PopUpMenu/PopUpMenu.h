#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Base2DEngine;

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class PopUpMenu : public GuiElementCore
{
public:

	// Constructor.
	PopUpMenu(std::string name, int imguiWindowFlags);
	// Destructor.
	virtual ~PopUpMenu();
	
	// Set the initial position.
	void setInitialPosition(const glm::vec2& pos);

	// Set the engine that the popup is in.
	void setEngine(Base2DEngine* engine);

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

private:

	// The position the popup has to be dispayed.
	glm::vec2 m_initialPos = { 0,0 };

	// The engine that the popup is in.
	Base2DEngine* m_engine = nullptr;

	// We need to close the popup on a defocus.
	virtual void onDefocusEvent(NotifyEvent& event) override;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//