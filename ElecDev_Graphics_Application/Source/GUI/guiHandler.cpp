//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "guiHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Constructor.
GUIHandler::GUIHandler(stateMachine* states, GraphicsHandler* graphicsHandler)
{
    // Set the custom theme.
    setTheme();
 
	this->states = *states;
	this->states.toolsExpanded = false;
	this->states.toolsMode = 0;
	this->textureID = 0;

	this->graphicsHandler = graphicsHandler;

	this->toolbar = new Toolbar(this->states);
	this->ribbons = new Ribbons(this->states);
	this->graphics = new Graphics(states, this->graphicsHandler);
	
};

void GUIHandler::renderGraphics()
{
	


	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
	ImVec2 work_size = viewport->WorkSize;
	ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y + 50));
	ImGui::SetNextWindowSize(ImVec2(work_size.x, work_size.y - 50));
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	bool p_open = true;
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);

	this->toolbar->renderToolbar();

	ImGui::SetNextWindowDockID(0, ImGuiCond_Once);

	this->ribbons->renderRibbons();

	this->graphics->renderGraphics();

	ImGui::Begin("FPS");
	ImGui::SetWindowPos(ImVec2(work_pos.x + work_size.x - 100, work_pos.y));
	ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	
	ImGui::End();	

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
