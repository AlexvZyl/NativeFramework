/*=======================================================================================================================================*/
/* Includes                                                                                                                              */
/*=======================================================================================================================================*/

#include "graphics.h"
#include <Core/imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cfenv>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
Graphics::Graphics(stateMachine* states, GraphicsHandler* graphicsHandler)
	: states(states), graphicsHandler(graphicsHandler) 
{

	this->pos.x = 0;
	this->pos.y = 0;

}


// Render the graphics scene.
void Graphics::renderGraphics() {

	ImGui::Begin("Render Window");
	{
		ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_Once);
		// Using a Child allow to fill all the space of the window.
		// It also allows customization
		ImGui::BeginChild("Render");
		this->states->renderWindowHovered = ImGui::IsWindowHovered();
		ImVec2 temp = ImGui::GetIO().MousePos;
		temp.x -= ImGui::GetWindowPos().x;
		temp.y -= ImGui::GetWindowPos().y;
		this->states->renderWindowMouseCoordinate = temp;

		ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(0), ImGuiCond_Once);


		if (ImGui::GetWindowSize().x != pos.x || ImGui::GetWindowSize().y != pos.y) {
			states->renderResizeEvent = true;
			states->renderWindowSize = ImGui::GetWindowSize();
		}
		pos.x = ImGui::GetWindowSize().x;
		pos.y = ImGui::GetWindowSize().y;


		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)this->graphicsHandler->m_drawingEngine->getRenderedTexID(), wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(4), ImGuiCond_Once);
	ImGui::End();

}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/
