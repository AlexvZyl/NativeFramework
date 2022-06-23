#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Windows/LumenWindow.h"
#include "Engines/CircuitDesigner/ComponentDesigner.h"
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class Port;
class Component2D;
class Cable;

//==============================================================================================================================================//
//  Color Editor.																																//
//==============================================================================================================================================//

class ComponentDesignerColorEditor : public LumenWindow
{
public:

	// Constructor.
	ComponentDesignerColorEditor(std::string name, int imguiWindowFlags = 0, glm::vec4* const target = nullptr);
	//ComponentDesignerColorEditor(std::string name, glm::vec4* const target, int imguiWindowFlags = 0);
	// Destructor.
	inline virtual ~ComponentDesignerColorEditor() = default;

	// Rendering.
	virtual void onImGuiBegin() override;
	virtual void onImGuiRender() override;
	virtual void onImGuiEnd() override;

	// Set the position of the editor.
	inline ComponentDesignerColorEditor& setInitialPosition(const glm::vec2& pos)
	{
		m_initialPosition = pos;
		return *this;
	}

private:


	// The position that the window starts up at.
	glm::vec2 m_initialPosition;
	glm::vec4* const m_target;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//