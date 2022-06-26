//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ComponentDesignerColorEditor.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Engines/CircuitDesigner/Peripherals/Cable.h"
#include "OpenGL/Primitives/Primitive.h"
#include "Graphics/Entities/Entity.h"
#include "Engines/CircuitDesigner/Peripherals/Port.h"
#include "Application/ApplicationTemplates.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/Text.h"
#include "Engines/CommandSystem/Command.h"

//==============================================================================================================================================//
//  Popup menu.																																	//
//==============================================================================================================================================//

ComponentDesignerColorEditor::ComponentDesignerColorEditor(std::string name, int imguiWindowFlags, glm::vec4* const target)
	: LumenWindow(name, imguiWindowFlags), m_target(target), color(*target)
{
	addImGuiWindowFlags(ImGuiWindowFlags_AlwaysAutoResize);
}


void ComponentDesignerColorEditor::onImGuiBegin()
{
	ImGui::SetNextWindowPos(m_initialPosition, ImGuiCond_Once);
	ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void ComponentDesignerColorEditor::onImGuiRender()
{
	ComponentDesigner* engine = Lumen::getApp().getActiveEngine<ComponentDesigner>();
	if (!engine) 
	{
		ImGui::Text("No active engine.");
		return;
	}

	// Get the active element.
	IPrimitive* activePrimitive = engine->m_activePrimitive;
	Port* activePort = engine->m_activePort.get();
	//Cable* activeCable = nullptr;

	// Set the color to be edited.
	
	if (activePrimitive != lastActivePrimitive) {
		if (!activePrimitive) glm::vec4 color = *m_target;
		else {
			if (activePrimitive)
			{
				color = activePrimitive->m_colour;
			}
		}
		lastActivePrimitive = activePrimitive;
		oldColor = color;
	}


	// Open color editor.
	ImGui::SameLine();
	if (ImGui::ColorPicker4("##ColorEditor", &color.r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf, &oldColor.r))
	{

		//No longer supported. To remove.
		/*
		else if (activeCable)
		{
			activeCable->m_colour = *color;
			activeCable->setColour(*color);
		}
		else if (activePort)
		{
			activePort->body->setColor(*color);
		}*/


		if(!activePrimitive) *m_target = color;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left)) {

	}
	else {
		if (activePrimitive)
		{
			if (oldColor != color)
				engine->commandLog.execute<SetColourCommand>(color, activePrimitive, oldColor);
			//activePrimitive->setColor(*color);
		}
		oldColor = color;
	}
	/*if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)) {
		if (activePrimitive)
		{
			if(oldColor != color)
			engine->commandLog.execute<SetColourCommand>(color, activePrimitive, oldColor);
			//activePrimitive->setColor(*color);
		}
	}*/
}

void ComponentDesignerColorEditor::onImGuiEnd()
{
	ImGui::End();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//