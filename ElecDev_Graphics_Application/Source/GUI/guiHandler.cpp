//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "guiHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
GUIHandler::GUIHandler(stateMachineGraphics* states, GraphicsHandler* graphicsHandler)
{
    // Set the custom theme.
    setTheme();
 
	this->statesG = *states;
	this->states.toolsExpanded = false;
	this->states.toolsMode = 0;
	this->textureID = 0;

	this->graphicsHandle = graphicsHandler;

	this->states.toolsMode = 0;
	this->states.toolsExpanded = 0;
	
};

void GUIHandler::renderGraphics()
{
	this->toolbar.renderToolbar();
	this->ribbons.renderRibbons();
	//this->graphics.renderGraphics();
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
	//ImGui::GetStyle().WindowRounding = 4.0f;
}
//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
