/*=======================================================================================================================================*/
/* Include.	                                                                                                                             */
/*=======================================================================================================================================*/

#include "mcc.h"
#include <Core/imgui.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include <string>
#include "Graphics/graphicsHandler.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
MCC::MCC(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), graphicsHandler(graphicsHandler)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;
}

// Render the graphics scene.
void MCC::renderGraphics(ImGuiID dock) 
{
	bool open = true;
	
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
					m_guiState->renderWindowHovered = ImGui::IsWindowHovered();
					struc.second->isHovered = ImGui::IsWindowHovered();
					ImVec2 temp = ImGui::GetIO().MousePos;
					temp.x -= ImGui::GetWindowPos().x;
					temp.y -= ImGui::GetWindowPos().y;
					struc.second->mouseCoords[0] = temp[0];
					struc.second->mouseCoords[1] = temp[1];

					if (ImGui::GetWindowSize().x != struc.second->viewportDimentions[0] || ImGui::GetWindowSize().y != struc.second->viewportDimentions[1]) 
					{
						struc.second->resizeEvent = true;
						ImVec2 temp = ImGui::GetWindowSize();
						struc.second->viewportDimentions[0] = temp[0];
						struc.second->viewportDimentions[1] = temp[1];
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
							graphicsHandler->m_activeWindow = "NULL";
						}
						// Reset mouse coordinates when it moves outside of the window.
						struc.second->engineGL->m_prevMouseEventWorldVec[0] = NULL;
						struc.second->engineGL->m_prevMouseEventWorldVec[1] = NULL;
						struc.second->engineGL->m_prevMouseEventWorldVec[2] = NULL;
						struc.second->engineGL->m_prevMouseEventPixelCoords[0] = NULL;
						struc.second->engineGL->m_prevMouseEventPixelCoords[1] = NULL;
					}

					//ImGui::SetWindowDock(ImGui::GetCurrentWindow(), ImGuiID(0), ImGuiCond_Once);

					if (ImGui::GetWindowSize().x != pos.x || ImGui::GetWindowSize().y != pos.y) {
						m_guiState->renderResizeEvent = true;
						ImVec2 temp = ImGui::GetWindowSize();
						m_guiState->renderWindowSize[0] = temp[0];
						m_guiState->renderWindowSize[1] = temp[1];
					}
					pos.x = ImGui::GetWindowSize().x;
					pos.y = ImGui::GetWindowSize().y;

					// Get the size of the child (i.e. the whole draw size of the windows).
					ImVec2 wsize = ImGui::GetWindowSize();

					// Because I use the texture from OpenGL, I need to invert the V from the UV.
					ImGui::Image((ImTextureID)struc.second->engineGL->getRenderTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

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
