#include "mcc.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "stateMachine.h"
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

/*=======================================================================================================================================*/
/* Additive functions                                                                                                                        */
/*=======================================================================================================================================*/

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

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
MCC::MCC(stateMachine* states, GraphicsHandler* graphicsHandler)
	: states(states), graphicsHandler(graphicsHandler)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;

	graphicsHandler->m_mccEngine->addMcc("Test");
}

void MCC::deQueueInput() {


	while (states->inputQueue.size() > 0) {

		inputQueue temp = states->inputQueue.front();

		switch (hash(temp.command.c_str())) {

		case hash("drawLine"):

			std::string mccName;
			float par[8];

			mccName = temp.parameters.substr(0, temp.parameters.find(";"));
			temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);

			for (size_t i = 0; i < 8; i++)
			{
				par[i] = std::stof(temp.parameters.substr(0, temp.parameters.find(";")));
				temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
			}

			graphicsHandler->m_mccEngine->drawLine(mccName,new float[2]{ par[0],par[1] }, new float[2]{ par[2],par[3] }, new float[4]{ par[4],par[5],par[6],par[7] });

			break;
		}

		states->inputQueue.pop();

	}

}

// Render the graphics scene.
void MCC::renderGraphics(ImGuiID dock) {

	//deQueueInput();
	bool open = true;

	//ImGui::SetNextWindowDockID(ImGuiID(1), ImGuiCond_Once);


	//ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(4), ImGuiCond_Once);
	
	if (graphicsHandler->m_mccEngine->m_mccDictionary.size() != 0) {
		for (auto struc : graphicsHandler->m_mccEngine->m_mccDictionary)
		{
			ImGui::SetNextWindowDockID(dock, ImGuiCond_Once);

			ImGui::Begin(struc.first.c_str());
			{

				//ImGui::GetCurrentWindow()->DockNode = ImGui::DockBuilderGetCentralNode(dock);

				//ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_Once);
				// Using a Child allow to fill all the space of the window.
				// It also allows customization
				ImGui::BeginChild("Render");
				this->states->renderWindowHovered = ImGui::IsWindowHovered();
				struc.second->isHovered = ImGui::IsWindowHovered();
				ImVec2 temp = ImGui::GetIO().MousePos;
				temp.x -= ImGui::GetWindowPos().x;
				temp.y -= ImGui::GetWindowPos().y;
				struc.second->mouseCoords = temp;

				if (ImGui::GetWindowSize().x != struc.second->viewportDimentions.x || ImGui::GetWindowSize().y != struc.second->viewportDimentions.y) {
					struc.second->resizeEvent = true;
					struc.second->viewportDimentions = ImGui::GetWindowSize();
				}

				// Set the active engine.
				if (struc.second->isHovered)
				{
					graphicsHandler->m_mccEngine->m_activeMCC = struc.first;
				}
				// Reset mouse coordinates.
				else
				{
					struc.second->engine->m_prevMouseEventWorldCoords[0] = NULL;
					struc.second->engine->m_prevMouseEventWorldCoords[1] = NULL;
				}

				//ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(0), ImGuiCond_Once);

				if (ImGui::GetWindowSize().x != pos.x || ImGui::GetWindowSize().y != pos.y) {
					states->renderResizeEvent = true;
					states->renderWindowSize = ImGui::GetWindowSize();
				}
				pos.x = ImGui::GetWindowSize().x;
				pos.y = ImGui::GetWindowSize().y;

				// Get the size of the child (i.e. the whole draw size of the windows).
				ImVec2 wsize = ImGui::GetWindowSize();
				// Because I use the texture from OpenGL, I need to invert the V from the UV.
				ImGui::Image((ImTextureID)struc.second->engine->getRenderedTexID(), wsize, ImVec2(0, 1), ImVec2(1, 0));
				

				ImGui::EndChild();
			}

			ImGui::End();


		}
	}
	

}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/
