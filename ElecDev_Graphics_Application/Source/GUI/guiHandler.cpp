/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

// ImGUI (GUI software). 
#include "Core/imgui.h"
#include "Implementations/imgui_impl_glfw.h"
#include "Implementations/imgui_impl_opengl3.h"
// GUI Components.
#include "Ribbons/ribbons.h"
#include "Toolbar/toolbar.h"
#include "GraphicsScene/GraphicsScene.h"
#include "../GUI/GUIState.h"
#include "UserGUI/userGUI.h"
#include "GuiHandler.h"
#include "Graphics/OpenGL/CoreGL/ErrorHandlerGL.h"
#include "ComponentEditor/ComponentEditor.h"
#include "PopUpMenu/PopUpMenu.h"
#include "CircuitEditor/CircuitEditor.h"

/*=======================================================================================================================================*/
/* Constructor & Destructor.																											 */
/*=======================================================================================================================================*/

GUIHandler::GUIHandler(GUIState* guiState, GraphicsHandler* graphicsHandler, PyInterface* pyInterface)
	:m_guiState(guiState), m_pyInterface(pyInterface)
{
	// Init variables.
	m_guiState->toolsExpanded = false;
	this->textureID = 0;
	this->graphicsHandler = graphicsHandler;

	// Create the GUI components.
	m_toolbar			= std::make_unique<Toolbar>(m_guiState);
	m_ribbons			= std::make_unique<Ribbons>(m_guiState, graphicsHandler);
	m_userGUIP			= std::make_unique<userGUI>(m_guiState, this->graphicsHandler, m_pyInterface);
	m_popUpMenu			= std::make_unique<PopUpMenu>(guiState, graphicsHandler);
	m_graphicsScene		= std::make_unique<GraphicsScene>(m_guiState, graphicsHandler);
	m_componentEditor	= std::make_unique<ComponentEditor>(m_guiState, graphicsHandler);
	m_circuitEditor		= std::make_unique<CircuitEditor>(m_guiState, graphicsHandler);

	// Set the custom theme.
	setTheme();
};

GUIHandler::~GUIHandler() {}

/*=======================================================================================================================================*/
/* Rendering.																															 */
/*=======================================================================================================================================*/

// [MAIN LOOP] Render the GUI to the screen.
void GUIHandler::renderGui(ImGuiIO& io, GLFWwindow* window)
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Update mouse position.
	m_guiState->imguiGlobalMouseCoords = ImGui::GetMousePos();

	// Assign values to viewport for ImGUI.
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	GLCall(glViewport(0, 0, display_w, display_h));

	// Feed inputs to ImGUI, start new frame.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Begin Docking Space.
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
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	bool p_open = true;
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);

	// End Docking space
	createDock(work_size);

	// Add latest docking branch before this can be used.
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	// Push own font.
	ImGui::PushFont(m_fontRobotoMedium);

	// ----------------------------- //
	//  G U I   C O M P O N E N T S  //
	// ----------------------------- //

	// Render GUI components.
	m_toolbar->renderToolbar();
	m_ribbons->renderRibbons(&this->dock);
	m_userGUIP->renderUI(&this->dock);
	
	// Circuit editor.
	if (m_guiState->circuitEditor)		{ m_circuitEditor->render(); }
	// Render OpenGL contexts.
	if (m_guiState->showGraphicsWindow) { m_graphicsScene->renderGraphics(this->dock); }
	// Render OpenGL context helper GUI's.
	if (m_guiState->popUpMenu)		    { m_popUpMenu->render(); }
	if (m_guiState->componentEditor)    { m_componentEditor->render(); }

	// ---------------- //
	//   C L E A N U P  //
	// ---------------- //

	ImGui::PopFont();
	ImGui::End();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

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

/*=======================================================================================================================================*/
/* Docking.																																 */
/*=======================================================================================================================================*/

void GUIHandler::createDock(ImVec2 work_size)
{
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//ImGui::DockBuilderRemoveNode(dockspace_id);

	if (m_userGUIP->resetDock || m_ribbons->first[0])
	{
		resetDock(dockspace_id);
		m_ribbons->first[0] = false;
	}
	else {
		ImGuiID dock = ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		this->dock = dock;
		this->dockSpaceID = dockspace_id;
	}
}

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

void GUIHandler::resetDock(ImGuiID dockspace_id) {

	ImGui::DockBuilderRemoveNode(dockspace_id);
	ImGuiID dock = ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::DockBuilderSplitNode(dock, ImGuiDir_Left, 0.1f, &this->m_ribbons->left, &dock);

	std::list<guiHolder>::iterator it = m_userGUIP->guis.begin();
	while (it != m_userGUIP->guis.end())
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
	m_userGUIP->resetDock = false;
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/