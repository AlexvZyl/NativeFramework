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

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
	ImVec2 work_size = viewport->WorkSize;
	ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y + 50));
	ImGui::SetNextWindowSize(ImVec2(work_size.x, work_size.y - 50));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	bool p_open = true;
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(2);
	ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");


	
	ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

	ImGuiID dock_main_id = dockspaceID;
	ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
	ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
	ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
	ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
	ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

	ImGui::DockBuilderDockWindow("Actions", dock_up_id);
	ImGui::DockBuilderDockWindow("Hierarchy", dock_right_id);
	ImGui::DockBuilderDockWindow("Inspector", dock_left_id);
	ImGui::DockBuilderDockWindow("Console", dock_down_id);
	ImGui::DockBuilderDockWindow("Project", dock_down_right_id);
	ImGui::DockBuilderDockWindow("Scene", dock_main_id);

	ImGui::DockBuilderFinish(dock_main_id);
	

	ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	this->toolbar->renderToolbar();

	//ImGui::SetNextWindowDockID(0, ImGuiCond_Once);

	this->ribbons->renderRibbons();

	this->graphics->renderGraphics();

	ImGui::Begin("FPS");
	ImGui::SetWindowPos(ImVec2(work_pos.x + work_size.x - 100, work_pos.y));
	ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	
	ImGui::End();	

	ImGui::End();

	ImGui::PopStyleVar();

};

void GUIHandler::setTheme() 
{
    //imGuiIO.Fonts->AddFontFromFileTTF("../data/Fonts/Ruda-Bold.ttf", 15.0f, &config);
    ImGui::GetStyle().FrameRounding = 4.0f;
    ImGui::GetStyle().GrabRounding = 4.0f;
	ImGui::GetStyle().ChildRounding = 4.0f;
}
//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
