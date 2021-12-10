/*=======================================================================================================================================*/
/* Include.	                                                                                                                             */
/*=======================================================================================================================================*/

#include "GraphicsScene.h"
#include <Core/imgui.h>
#include <iostream>
#include <cmath>
#include <cfenv>
#include <string>
#include "Graphics/graphicsHandler.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Core/imgui_internal.h>
#include "../PopUpMenu/PopUpMenu.h"

#include "../GUIState.h"


/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
GraphicsScene::GraphicsScene(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), graphicsHandler(graphicsHandler)
{
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;
}

GraphicsScene::~GraphicsScene(){}

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Render the graphics scene.
void GraphicsScene::renderGraphics(ImGuiID dock)
{
	bool open = true;
	if (graphicsHandler->m_windowsDictionary.size() != 0) {
		std::vector<std::string> toRemove;

		// ----------------------------- //
		//  R E N D E R   W I N D O W S  //
		// ----------------------------- //

		// Iterate through all of the render windows.
		for (auto const& [name, window] : graphicsHandler->m_windowsDictionary)
		{
			ImGui::SetNextWindowDockID(dock, ImGuiCond_Once);
			if (window->close)
			{
				if (ImGui::Begin(window->engineGL->m_contextName.c_str(), &window->close))
				{
					// Using a Child allow to fill all the space of the window.
					// It also allows customization
					std::string childName = name + "Render";
					ImGui::BeginChild(childName.c_str());
					m_guiState->renderWindowHovered = ImGui::IsWindowHovered();
					window->isHovered = ImGui::IsWindowHovered();
					window->isFocused = ImGui::IsWindowFocused();
					ImVec2 temp = ImGui::GetIO().MousePos;
					window->mouseCoords[0] = temp.x - ImGui::GetWindowPos().x;
					window->mouseCoords[1] = temp.y - ImGui::GetWindowPos().y;
					if (ImGui::GetWindowSize().x != window->viewportDimentions[0] || ImGui::GetWindowSize().y != window->viewportDimentions[1])
					{
						window->resizeEvent = true;
						ImVec2 temp = ImGui::GetWindowSize();
						window->viewportDimentions[0] = temp[0];
						window->viewportDimentions[1] = temp[1];
					}

					// ----------- //
					//  F O C U S  //
					// ----------- //

					// Set the active engineGL.
					if (window->isFocused && window->isHovered)
					{ graphicsHandler->m_activeWindow = graphicsHandler->m_windowsDictionary[name]; }
					// If the window is not focused.
					else
					{
						// If the active engineGL is the current engineGL, disable.
						if (graphicsHandler->isActiveWindowValid()) {
							if (name == graphicsHandler->m_activeWindow->windowName) {
								graphicsHandler->m_activeWindow = NULL;
							}
						}

						// Reset mouse coordinates when it moves outside of the window.
						window->engineGL->m_prevMouseEventWorldVec[0] = NULL;
						window->engineGL->m_prevMouseEventWorldVec[1] = NULL;
						window->engineGL->m_prevMouseEventWorldVec[2] = NULL;
						window->engineGL->m_prevMouseEventPixelCoords[0] = NULL;
						window->engineGL->m_prevMouseEventPixelCoords[1] = NULL;
					}

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
					ImGui::Image((ImTextureID)window->engineGL->getRenderTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

					// End.
					ImGui::EndChild();
				}
				ImGui::End();
			}
			// Add window to remove.
			else 
			{ toRemove.push_back(name.c_str()); }
		}
		// Remove windows that have been closed.
		for (auto remove : toRemove) { graphicsHandler->removeWindow(remove); }
	}
}

/*=======================================================================================================================================*/
/* EOF                                                                                                                                   */
/*=======================================================================================================================================*/