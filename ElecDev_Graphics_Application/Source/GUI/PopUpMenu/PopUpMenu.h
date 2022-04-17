#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/LumenWindow/LumenWindow.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Base2DEngine;

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

class PopUpMenu : public LumenWindow
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

	// Dispatch events.
	virtual void onEvent(const Event& event) override;

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

private:

	// The position the popup has to be dispayed.
	glm::vec2 m_initialPos = { 0,0 };

	// The engine that the popup is in.
	Base2DEngine* m_engine = nullptr;

	// We need to close the popup on a defocus.
	virtual void onDefocusEvent(const NotifyEvent& event);
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//