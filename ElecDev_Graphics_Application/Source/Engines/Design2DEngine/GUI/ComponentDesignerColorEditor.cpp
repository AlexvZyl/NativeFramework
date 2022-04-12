//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ComponentDesignerColorEditor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"
#include "OpenGL/Primitives/Primitive.h"
#include "Graphics/Entities/Entity.h"
#include "Engines/Design2DEngine/Peripherals/Port.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

ComponentDesignerColorEditor::ComponentDesignerColorEditor(std::string name, int imguiWindowFlags)
	: GuiElementCore(name, imguiWindowFlags)
{}

void ComponentDesignerColorEditor::begin()
{
	ImGui::SetNextWindowSize(glm::vec2(400, 350), ImGuiCond_Once);
	ImGui::SetNextWindowPos(m_initialPosition, ImGuiCond_Once);
	ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ComponentDesignerColorEditor::onRender()
{
	ComponentDesigner* engine = Lumen::getApp().getActiveEngine<ComponentDesigner>();
	if (!engine) 
	{
		ImGui::Text("No active engine.");
		return;
	}

	// Get the active element.
	PrimitivePtr* activePrimitive = nullptr;
	Port* activePort = nullptr;
	Cable* activeCable = nullptr;
	if (engine->m_activeCircle)
	{
		activePrimitive = engine->m_activeCircle;
	}
	else if(engine->m_activeLine) 
	{
		activePrimitive = engine->m_activeLine;
	}
	else if (engine->m_activePoly) 
	{
		activePrimitive = engine->m_activePoly;
	}
	else if (engine->m_activeText) 
	{
		activePrimitive = engine->m_activeText;
	}
	else if (engine->m_activeLine) 
	{
		activePrimitive = engine->m_activeLine;
	}
	else if(engine->m_activePort)
	{
		activePort = engine->m_activePort.get();
	}
	else if (false) 
	{
		// Should be the active cable.
	}

	// Set the color to be edited.
	glm::vec4* color = nullptr;
	if (activePrimitive)
	{
		color = &activePrimitive->m_colour;
	}
	else if (activePort) 
	{
		color = &activePort->body->m_colour;
	}
	else if (activeCable)
	{
		color = &activeCable->m_colour;
	}

	// Open color editor.
	if (color)
	{
		ImGui::SameLine();
		if (ImGui::ColorPicker4("##ColorEditor", &color->r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			if (activePrimitive)
			{
				activePrimitive->setColor(*color);
			}
			else if (activeCable)
			{
				activeCable->m_colour = *color;
				activeCable->setColour(*color);
			}
			else if (activePort) 
			{
				activePort->body->setColor(*color);
			}
		}
	}
	else 
	{
		ImGui::Text("No active element.");
	}
}

void ComponentDesignerColorEditor::end()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//