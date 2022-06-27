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
	std::vector<IPrimitive*> activePrimitives = engine->m_activePrimitives;
	//Port* activePort = engine->m_activePort.get();
	//Cable* activeCable = nullptr;

	// Set the color to be edited.
	if (activePrimitives.size() != oldColor.size()) {
		oldColor.clear();
		for (auto primitive : activePrimitives)
			oldColor.push_back(primitive->m_colour);
	}
	if (!activePrimitives.empty()) {
		if (activePrimitives.front() != lastActivePrimitive) {
			color = activePrimitives.front()->m_colour;
			oldColor.clear();
			for (auto primitive : activePrimitives)
				oldColor.push_back(primitive->m_colour);
		}
		lastActivePrimitive = activePrimitives.front();
	}
	else {
		color = *m_target;
		oldColor.clear();
		oldColor.push_back(color);
	}


	// Open color editor.
	ImGui::SameLine();
	if (ImGui::ColorPicker4("##ColorEditor", &color.r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf, &oldColor.front().r))
	{

		//No longer supported. TODO: remove.
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


		if(activePrimitives.empty()) *m_target = color;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left)) {

	}
	else {
		// TODO: implement this with a batch command
		for(int i = 0; i < activePrimitives.size(); i++)
		{
			if (oldColor.at(i) != color)
				engine->commandLog.execute<SetColourCommand>(color, activePrimitives.at(i), oldColor.at(i));
			//activePrimitive->setColor(*color);
		}
		for(auto& c : oldColor)
			c = color;
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