//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

//  General.
#include <string>
#include <iostream>

#include "guiHandler.h"
#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "../Helper/stateMachine.h"

// Constructor.
GUIHandler::GUIHandler(stateMachineGraphics* states, GraphicsHandler* graphicsHandler)
{
    // Set the custom theme.
    setTheme();
 
	this->statesG = *states;
	this->states.toolsExpanded = false;
	this->states.toolsMode = 0;

	this->graphics = graphicsHandler;
	
};

void GUIHandler::renderGraphics()
{
	this->toolbar.renderToolbar();
	this->ribbons.renderRibbons();
	ImGui::Begin("FPS");
	ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::SetWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x -160, 0));
	ImGui::SetNextWindowSize(ImVec2(30, 10));
	ImGui::End();	
};

void GUIHandler::setTheme() 
{
    //imGuiIO.Fonts->AddFontFromFileTTF("../data/Fonts/Ruda-Bold.ttf", 15.0f, &config);
    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().GrabRounding = 4.0f;
	ImGui::GetStyle().ChildRounding = 4.0f;
	ImGui::GetStyle().WindowRounding = 4.0f;




}