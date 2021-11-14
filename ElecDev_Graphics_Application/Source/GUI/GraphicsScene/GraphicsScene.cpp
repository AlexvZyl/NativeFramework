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

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Constructor.
GraphicsScene::GraphicsScene(GUIState* guiState, GraphicsHandler* graphicsHandler)
	: m_guiState(guiState), graphicsHandler(graphicsHandler)
{
	m_popUpMenu = new PopUpMenu(guiState);
	this->pos.x = 0;
	this->pos.y = 0;
	this->dock = 0;
}

GraphicsScene::~GraphicsScene()
{
	delete m_popUpMenu;
}

/*=======================================================================================================================================*/
/* Declarations                                                                                                                          */
/*=======================================================================================================================================*/

// Render the graphics scene.
void GraphicsScene::renderGraphics(ImGuiID dock)
{
	bool open = true;
	if (graphicsHandler->m_windowsDictionary.size() != 0) {
		std::vector<std::string> toRemove;
		for (auto const& [name, window] : graphicsHandler->m_windowsDictionary)
		{
			ImGui::SetNextWindowDockID(dock, ImGuiCond_Once);
			if (window->close)
			{
				if (ImGui::Begin(window->windowName.c_str(), &window->close))
				{
					// Using a Child allow to fill all the space of the window.
					// It also allows customization
					std::string childName = name + "Render";
					ImGui::BeginChild(childName.c_str());
					m_guiState->renderWindowHovered = ImGui::IsWindowHovered();
					window->isHovered = ImGui::IsWindowHovered();
					window->isFocused = ImGui::IsWindowFocused();
					ImVec2 temp = ImGui::GetIO().MousePos;
					temp.x -= ImGui::GetWindowPos().x;
					temp.y -= ImGui::GetWindowPos().y;
					window->mouseCoords[0] = temp[0];
					window->mouseCoords[1] = temp[1];
					if (ImGui::GetWindowSize().x != window->viewportDimentions[0] || ImGui::GetWindowSize().y != window->viewportDimentions[1])
					{
						window->resizeEvent = true;
						ImVec2 temp = ImGui::GetWindowSize();
						window->viewportDimentions[0] = temp[0];
						window->viewportDimentions[1] = temp[1];
					}
					// Set the active engineGL.
					if (window->isFocused)
					{
						graphicsHandler->m_activeWindow = graphicsHandler->m_windowsDictionary[name];
					}
					// Reset mouse coordinates.
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

					// Popup Menu.
					if (m_guiState->popUpMenu) { m_popUpMenu->render(); }

					// End.
					ImGui::EndChild();
				}
				ImGui::End();
			}
			else {
				toRemove.push_back(name.c_str());
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