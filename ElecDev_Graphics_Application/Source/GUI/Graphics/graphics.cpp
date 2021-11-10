/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include "graphics.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include "GUI/GUIState.h"
#include "PythonInterface.h"

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
Graphics::Graphics(GUIState* guiState, GraphicsHandler* graphicsHandler, PyInterface* pyInterface)
	: m_guiState(guiState), graphicsHandler(graphicsHandler), m_pyInterface(pyInterface)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;
}

void Graphics::deQueueInput() 
{
	while (m_pyInterface->inputQueue.size() > 0)
	{
		InputQueue temp = m_pyInterface->inputQueue.front();
		switch (hash(temp.command.c_str())) {
			case hash("drawLine"):
				float par[8];
				for (size_t i = 0; i < 8; i++)
				{
					par[i] = std::stof(temp.parameters.substr(0, temp.parameters.find(";")));
					temp.parameters = temp.parameters.substr(temp.parameters.find(";") + 1);
				}
				break;
		}
		m_pyInterface->inputQueue.pop();
	}
}

// Render the graphics scene.
void Graphics::renderGraphics(ImGuiID dock) {

	deQueueInput();
	bool open = true;
	ImGui::SetNextWindowDockID(dock, ImGuiCond_Once);

	ImGui::Begin("Render Window");
	{
		
		//ImGui::GetCurrentWindow()->DockNode = ImGui::DockBuilderGetCentralNode(dock);

		//ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_Once);
		// Using a Child allow to fill all the space of the window.
		// It also allows customization
		ImGui::BeginChild("Render");
		this->m_guiState->renderWindowHovered = ImGui::IsWindowHovered();
		ImVec2 temp = ImGui::GetIO().MousePos;
		temp.x -= ImGui::GetMousePos().x;
		temp.y -= ImGui::GetMousePos().y;
		m_guiState->renderWindowMouseCoordinate[0] = temp.x;
		m_guiState->renderWindowMouseCoordinate[1] = temp.y;

		//ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(0), ImGuiCond_Once);

		if (ImGui::GetWindowSize().x != pos.x || ImGui::GetWindowSize().y != pos.y) {
			m_guiState->renderResizeEvent = true;
			ImVec2 temp = ImGui::GetWindowSize();
			m_guiState->renderWindowSize[0] = temp.x;
			m_guiState->renderWindowSize[1] = temp.y;
		}
		pos.x = ImGui::GetWindowSize().x;
		pos.y = ImGui::GetWindowSize().y;

		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		//ImGui::Image((ImTextureID)this->graphicsHandler->m_baseEngine->getRenderedTexID(), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();
}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/
