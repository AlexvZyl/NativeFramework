#include "userGUI.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

// Hash the input text to be checked against list of commands.
constexpr size_t hash(const char* str) {
	const long long p = 131;
	const long long m = 4294967291; // 2^32 - 5, largest 32 bit prime
	long long total = 0;
	long long current_multiplier = 1;
	for (int i = 0; str[i] != '\0'; ++i) {
		total = (total + current_multiplier * str[i]) % m;
		current_multiplier = (current_multiplier * p) % m;
	}
	return total;
}

// Constructor.
userGUI::userGUI(stateMachine* states, GraphicsHandler* graphicsHandler)
	: states(states), graphicsHandler(graphicsHandler)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;

}

void userGUI::pushData(std::list<guiHolder>::iterator uiWindow) {

	std::cout << "Pushing data" << std::endl;
	std::list<element>::iterator elements = uiWindow->elements.begin();
	std::string output;
	output.append("[");
	output.append(uiWindow->windowName);
	output.append("][");
	while (elements != uiWindow->elements.end())
	{
		output.append("<");
		output.append(elements->type.c_str());
		output.append(";");
		output.append(elements->name.c_str());
		output.append(";");
		output.append(elements->data.c_str());
		output.append(";");
		output.append(">");
		++elements;

	}
	output.append("]");

	outputQueue tempQueue("ReturnUI", output);
	states->outputQ.push(tempQueue);

}

void userGUI::createGUI(std::string guiName, std::string guiPos, std::string docking, std::string parameters) {

	size_t pos = 0;
	size_t pos_2 = 0;
	std::string delimiter = ";";
	std::list<element> elements;
	std::string s = parameters;

	std::string token;
	// Add element loop


	while ((pos = s.find(delimiter)) != std::string::npos) {

		std::string elementTemp = s.substr(0, pos);
		int pos1 = elementTemp.find("<");

		std::string elementType = elementTemp.substr(0, pos1);

		elementTemp.erase(0, pos1 + 1);

		int pos2 = elementTemp.find(">");

		std::string elementName = elementTemp.substr(0, pos2);

		elementTemp.erase(0, pos2 + 2);

		std::list<std::string> extra;

		while ((pos_2 = elementTemp.find(">")) != std::string::npos) {
			extra.push_back(elementTemp.substr(0, pos_2));
			elementTemp.erase(0, pos_2 + 2);
		}
		elementTemp = elementTemp.erase(0, pos_2);

		s.erase(0, pos + delimiter.length());
		element tempElement(elementType, elementName, extra);
		if (elementType == "InputText") {
			tempElement.data = extra.front().c_str();
		}
		
		elements.push_back(tempElement);
	}

	ImVec2 winPos;

	winPos.x = std::stof(guiPos.substr(0, guiPos.find(",")));
	winPos.y = std::stof(guiPos.substr(guiPos.find(",") + 1, guiPos.size() - guiPos.find(",") - 1));

	guiHolder temp(guiName, winPos, docking, elements);
	temp.docking = docking;
	temp.addDock = true;
	guis.push_back(temp);
	resetDock = true;
}

void userGUI::renderUI(ImGuiID* dock) {

	std::list<guiHolder>::iterator it = this->guis.begin();
	while (it != guis.end())
	{

		if (it->submit)
		{
			pushData(it);
		}

		if (it->close)
		{
			if (it->docked) {
				ImGui::SetNextWindowDockID(it->dockPos, ImGuiCond_Once);
			}
			else {
				ImGui::SetNextWindowPos(it->windowPos, ImGuiCond_Once);
			}


			if (ImGui::Begin(it->windowName.c_str(), &it->close)) {
				ImGui::SetWindowSize(ImVec2(0, 0));


				std::list<element>::iterator it2;
				for (it2 = it->elements.begin(); it2 != it->elements.end(); ++it2)
				{
					static char str0[128] = "";
					static int sliderVal = 0;
					switch (hash(it2->type.c_str()))
					{

					case hash("Button"):
					{
						if (ImGui::Button(it2->name.c_str())) {

							it2->data.clear();
							it2->data.append("True");

						}
						else {
							it2->data.clear();
							it2->data.append("False");
						}
						break;
					}

					case hash("SButton"):
					{
						if (ImGui::Button(it2->name.c_str())) {
							it2->data.clear();
							it2->data.append("True");
							it->submit = true;
							it->close = false;

						}
						else {
							it2->data.clear();
							it2->data.append("False");
						}
						break;
					}

					case hash("Text"):
					{
						ImGui::Text(it2->name.c_str());
						break;
					}

					case hash("InputText"):
					{
						strcpy_s(str0, it2->data.c_str());
						ImGui::InputText(it2->name.c_str(), str0, IM_ARRAYSIZE(str0));
						std::string out = str0;
						it2->data.clear();
						it2->data.append(out);
						break;
					}
					case hash("SameLine"):
					{
						ImGui::SameLine();
						break;
					}
					case hash("SliderAngle"):
					{


						int range[2];
						int count = 0;
						for (std::list<std::string>::iterator it3 = it2->extraParams.begin(); it3 != it2->extraParams.end(); ++it3)
						{
							range[count] = std::stof(it3->c_str());
							count++;
						}

						ImGui::SliderInt(it2->name.c_str(), &sliderVal, range[0], range[1]);
						it2->data = std::to_string(sliderVal);
						break;
					}
					case hash("ComboBox"):
					{

						static int item_current_idx = 0; // Here we store our selection data as an index.
						int count = 0;
						const char* combo_preview_value = it2->data.c_str();
						if (ImGui::BeginCombo(it2->name.c_str(), combo_preview_value))
						{

							for (std::list<std::string>::iterator it3 = it2->extraParams.begin(); it3 != it2->extraParams.end(); ++it3)
							{
								const bool is_selected = (item_current_idx == count);
								if (ImGui::Selectable(it3->c_str(), is_selected)) {
									item_current_idx = count;
									it2->data = it3->c_str();
								}

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected) {
									ImGui::SetItemDefaultFocus();
								}
								count++;
							}
							ImGui::EndCombo();
						}
						break;
					}

					case hash("TickBox"):
					{
						static bool ticked = true;
						ImGui::Checkbox(it2->name.c_str(), &ticked);

						if (ticked) {
							it2->data.clear();
							it2->data.append("True");
						}
						else {
							it2->data.clear();
							it2->data.append("False");
						}

					}



					default:
					{
						break;
					}
					}

				}
				ImGui::End();

			}

			++it;
		}
		else {
			guis.erase(it++);
		}
	}
	
}