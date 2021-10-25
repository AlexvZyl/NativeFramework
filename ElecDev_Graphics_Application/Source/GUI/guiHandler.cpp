//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "guiHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// Hash the input text to be checked against list of commands.
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

// Constructor.
GUIHandler::GUIHandler(stateMachine* states, GraphicsHandler* graphicsHandler)
{
    // Set the custom theme.
    setTheme();
 
	this->states = states;
	this->states->toolsExpanded = false;
	this->states->toolsMode = 0;
	this->textureID = 0;

	this->graphicsHandler = graphicsHandler;

	this->toolbar = new Toolbar(this->states);
	this->ribbons = new Ribbons(this->states);
	this->graphics = new Graphics(states, this->graphicsHandler);
	this->userGUIP = new userGUI(states, this->graphicsHandler);

	//MCCDict.

	this->mcc = new MCC(states,graphicsHandler);
	//MCCDict->insert("test", new BaseEngineGL(m_window, states));
	
};

// Destructor.
GUIHandler::~GUIHandler() 
{
	delete toolbar;
	delete ribbons;
	delete graphics;
	delete userGUIP;
	delete mcc;
}

// [MAIN LOOP] Render the GUI to the screen.
void GUIHandler::renderGui(ImGuiIO& io, GLFWwindow* window)
{
	// Assign values to viewport for ImGUI.
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	GLCall(glViewport(0, 0, display_w, display_h));

	// Feed inputs to ImGUI, start new frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Begion Docking Space

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
	work_pos.y = work_pos.y + 50;
	ImVec2 work_size = viewport->WorkSize;
	work_size.y = work_size.y - 50;
	ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y));
	ImGui::SetNextWindowSize(ImVec2(work_size.x, work_size.y));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	bool p_open = true;

	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(2);

	createDock(work_size);
	// End Docking space

	this->toolbar->renderToolbar();

	this->ribbons->renderRibbons(&this->dock);

	this->userGUIP->renderUI(&this->dock);

	if (states->showGraphicsWindow) {
		//this->graphics->renderGraphics(dock);
		this->mcc->renderGraphics(this->dock);
	}



	//ImGui::Begin("FPS");
	//ImGui::SetWindowPos(ImVec2(work_pos.x + work_size.x - 100, work_pos.y));
	//ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
	//ImGui::End();	

	ImGui::End();

	//this->userWindow->renderWindow(dock);

	ImGui::PopStyleVar();

	//Render ImGUI into screen.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
};

void GUIHandler::setTheme()
{
	//imGuiIO.Fonts->AddFontFromFileTTF("../data/Fonts/Ruda-Bold.ttf", 15.0f, &config);
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;
	ImGui::GetStyle().ChildRounding = 4.0f;
}

void GUIHandler::createDock(ImVec2 work_size) {

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//ImGui::DockBuilderRemoveNode(dockspace_id);

	if (this->userGUIP->resetDock || this->ribbons->first[0])
	{
		resetDock(dockspace_id);
		this->ribbons->first[0] = false;
	}
	else {
		ImGuiID dock = ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		this->dock = dock;
		this->dockSpaceID = dockspace_id;
	}

}

void GUIHandler::resetDock(ImGuiID dockspace_id) {

	ImGui::DockBuilderRemoveNode(dockspace_id);
	ImGuiID dock = ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::DockBuilderSplitNode(dock, ImGuiDir_Left, 0.1f, &this->ribbons->left, &dock);

	std::list<guiHolder>::iterator it = this->userGUIP->guis.begin();
	while (it != this->userGUIP->guis.end())
	{

		switch (hash(it->docking.c_str()))
		{
		case (hash("LEFT")):	
			ImGui::DockBuilderSplitNode(dock, ImGuiDir_Left, 0.2f, &it->dockPos, &dock);
			break;
		case (hash("RIGHT")):
			ImGui::DockBuilderSplitNode(dock, ImGuiDir_Right, 0.2f, &it->dockPos, &dock);
			break;
		case (hash("TOP")):
			ImGui::DockBuilderSplitNode(dock, ImGuiDir_Up, 0.2f, &it->dockPos, &dock);
			break;
		case (hash("BOTTOM")):
			ImGui::DockBuilderSplitNode(dock, ImGuiDir_Down, 0.2f, &it->dockPos, &dock);
			break;

		default:
			break;
		}
		it->docked = true;
		++it;
		
	}
	this->dock = dock;
	this->dockSpaceID = dockspace_id;
	this->userGUIP->resetDock = false;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------
