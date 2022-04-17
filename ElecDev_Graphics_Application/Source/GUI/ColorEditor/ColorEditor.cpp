//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ColorEditor.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Component2D.h"
#include "Engines/Design2DEngine/Peripherals/Cable.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

ColorEditor::ColorEditor(std::string name, int imguiWindowFlags) 
	: LumenWindow(name, imguiWindowFlags)
{}

void ColorEditor::onImGuiBegin()
{
	ImGui::SetNextWindowSize(glm::vec2(400, 350), ImGuiCond_Once);
	ImGui::SetNextWindowPos(m_initialPosition, ImGuiCond_Once);
	ImGui::Begin(getImGuiName(), &m_isOpen, m_imguiWindowFlags);
}

void ColorEditor::onImGuiRender()
{
	Application& app = Lumen::getApp();

	// Get active elements.
	Design2DEngine* engine = app.getActiveEngine<Design2DEngine>();
	Component2D* component = nullptr;
	Cable* cable = nullptr;
	if (engine)
	{
		component = engine->m_activeComponent.get();
		cable = engine->m_activeCable.get();
	}

	// Set the color to be edited.
	glm::vec4* color = nullptr;
	if (component)
		color = &component->shapeColour;
	else if (cable)
	{
		color = &cable->m_colour;
		cable->setColour(*color, false);
	}

	// Open color editor.
	if (color)
	{
		ImGui::SameLine();
		if (ImGui::ColorPicker4("##ColorEditor", &color->r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			if (component)
			{
				component->setColour(*color);
			}
			if (cable)
			{
				cable->m_colour = *color;
				cable->setColour(*color);
			}
		}
	}
	else 
	{
		ImGui::Text("No active element.");
	}
}

void ColorEditor::onImGuiEnd() 
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//