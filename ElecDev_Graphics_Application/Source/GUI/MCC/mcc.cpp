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
/* Additive functions                                                                                                                    */
/*=======================================================================================================================================*/



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

	//graphicsHandler->addMcc("Test");
}

// Render the graphics scene.
void MCC::renderGraphics(ImGuiID dock) 
{
	bool open = true;

	//ImGui::SetNextWindowDockID(ImGuiID(1), ImGuiCond_Once);

	//ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(4), ImGuiCond_Once);
	
	if (graphicsHandler->m_windowsDictionary.size() != 0) {
		std::vector<std::string> toRemove;
		for (auto struc : graphicsHandler->m_windowsDictionary)
		{
			
			ImGui::SetNextWindowDockID(dock, ImGuiCond_Once);

			if (struc.second->close) {
				if (ImGui::Begin(struc.second->windowName.c_str(), &struc.second->close))
				{

					//ImGui::GetCurrentWindow()->DockNode = ImGui::DockBuilderGetCentralNode(dock);

					//ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_Once);
					// Using a Child allow to fill all the space of the window.
					// It also allows customization
					std::string childName = struc.first + "Render";
					ImGui::BeginChild(childName.c_str());
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

					// Set the active engineGL.
					if (struc.second->isHovered)
					{
						graphicsHandler->m_activeWindow = struc.first;
					}
					// Reset mouse coordinates.
					else
					{
						// If the active engineGL is the current engineGL, disable.
						if (struc.first == graphicsHandler->m_activeWindow)
						{
							graphicsHandler->m_activeWindow = 'NULL';
						}

						// Reset mouse coordinates when it moves outside of the window.
						struc.second->engineGL->m_prevMouseEventWorldCoords[0] = NULL;
						struc.second->engineGL->m_prevMouseEventWorldCoords[1] = NULL;
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
					ImGui::Image((ImTextureID)struc.second->engineGL->getRenderedTexID(), wsize, ImVec2(0, 1), ImVec2(1, 0));

					ImGui::EndChild();
				}
			ImGui::End();
			}
			else {
				toRemove.push_back(struc.first.c_str());
			}
		}
		for (auto remove : toRemove)
		{
			graphicsHandler->removeWindow(remove);
		}
	}
}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/