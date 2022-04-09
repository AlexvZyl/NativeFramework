#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "GUI/GuiElementCore/GuiElementCore.h"
#include "Engines/Design2DEngine/ComponentDesigner.h"
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

class ComponentDesignerColorEditor : public GuiElementCore
{
public:

	// Constructor.
	ComponentDesignerColorEditor(std::string name, int imguiWindowFlags);
	// Destructor.
	inline virtual ~ComponentDesignerColorEditor() = default;

	// Rendering.
	virtual void begin() override;
	virtual void onRender() override;
	virtual void end() override;

	// Set the position of the editor.
	inline ComponentDesignerColorEditor& setInitialPosition(const glm::vec2& pos)
	{
		m_initialPosition = pos;
		return *this;
	}

	// Set the engine related to this editor.
	inline ComponentDesignerColorEditor& setEngine(ComponentDesigner* engine)
	{
		m_engine = engine;
		return *this;
	}

private:

	// The engine related to this editor.
	ComponentDesigner* m_engine;

	// The position that the window starts up at.
	glm::vec2 m_initialPosition;
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//