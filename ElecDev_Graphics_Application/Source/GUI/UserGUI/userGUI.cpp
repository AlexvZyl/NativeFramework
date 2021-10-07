#include "userGUI.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

void userGUI::deQueueInput() {


	//while (states->inputQueue.size() > 0) {

	//	inputQueue temp = states->inputQueue.front();

	//	switch (hash(temp.command.c_str())) {

	//	case hash("drawLine"):

	//		float par[8];

	//		for (size_t i = 0; i < 8; i++)
	//		{
	//			par[i] = std::stof(temp.parameters.substr(0, temp.parameters.find(";")));
	//			temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
	//		}

	//		//graphicsHandler->m_baseEngine->drawLine(new float[2]{ par[0],par[1] }, new float[2]{ par[2],par[3] }, new float[4]{ par[4],par[5],par[6],par[7] });

	//		break;
	//	}

	//	states->inputQueue.pop();

	//}

}

// Constructor.
userGUI::userGUI(stateMachine* states, GraphicsHandler* graphicsHandler)
	: states(states), graphicsHandler(graphicsHandler)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;

}

// Render the graphics scene.
void userGUI::renderWindow(ImGuiID dock) {

	deQueueInput();

	//ImGui::SetNextWindowDockID(ImGuiID(1), ImGuiCond_Once);

	if (this->close) {

		if (ImGui::Begin("User Window", &this->close))
		{
			this->states->userWindowHovered = ImGui::IsWindowHovered();
			// User check if older version
			if (ImGui::Checkbox("User Previous", &this->userPrevious)) {

			}

			ImGui::Text("Voltage Levels");
			ImGui::SameLine();

			// Voltage drop down list
			const char* items[] = { "460", "525", "690" };
			if (ImGui::BeginCombo("##Voltage Levels", this->voltageLevel, ImGuiComboFlags_NoArrowButton))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (this->voltageLevel == items[n]);
					if (ImGui::Selectable(items[n], is_selected))
						this->voltageLevel = items[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Motor Efficiency");
			ImGui::SameLine();
			// motor efficiency ratings
			const char* items2[] = { "IEC", "IE4", "IE3", "IE2", "IE1" };
			if (ImGui::BeginCombo("##Motor Efficiency Ratings", this->efficiencyRating, ImGuiComboFlags_NoArrowButton))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (this->efficiencyRating == items2[n]);
					if (ImGui::Selectable(items2[n], is_selected))
						this->efficiencyRating = items2[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Enter Steady Volt Drop");
			ImGui::SameLine();
			ImGui::InputText("", this->steadyVoltDrop, IM_ARRAYSIZE(this->steadyVoltDrop));

			if (ImGui::Button("Save")) {



			}
			ImGui::End();

		}
		else {
			ImGui::End();
		}


	}

}