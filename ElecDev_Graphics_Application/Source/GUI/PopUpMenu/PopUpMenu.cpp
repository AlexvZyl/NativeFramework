/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "PopUpMenu.h"
#include <iostream>
#include "imgui/imgui.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/Design2DEngine/Design2DEngine.h"
#include "Engines/Design2DEngine/Peripherals/Circuit.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "GUI/CircuitEditor/CircuitEditor.h"
#include "GUI/ColorEditor/ColorEditor.h"

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

PopUpMenu::PopUpMenu(std::string name, int imguiWindowFlags)
    : GuiElementCore(name, imguiWindowFlags)
{
    m_imguiWindowFlags |= ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDocking;
}

PopUpMenu::~PopUpMenu() 
{
    Lumen::getApp().m_guiState->clickedZone.background = false;
    Lumen::getApp().m_guiState->clickedZone.component = false;
    Lumen::getApp().m_guiState->clickedZone.primative= false;
    Lumen::getApp().m_guiState->clickedZone.port = false;
}

void PopUpMenu::setInitialPosition(glm::vec2& pos) 
{
    m_initialPos = pos;
}

void PopUpMenu::setEngine(Design2DEngine* engine)
{
    m_engine = engine;
}

void PopUpMenu::begin()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 5.f });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.f, 6.f });
    ImGui::SetNextWindowPos(ImVec2(m_initialPos.x, m_initialPos.y));
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

/*=======================================================================================================================================*/
/* Rendering.																															 */
/*=======================================================================================================================================*/

// Render call.
void PopUpMenu::onRender()
{ 
    Application& app = Lumen::getApp();

    // --------------------- //
    //  B A C K G R O U N D  //
    // --------------------- //
    
    // Render menu items.
    if (!m_engine->m_activeComponent && !m_engine->m_activeCable)
    {
        if (ImGui::MenuItem("Place component", "P"))
        {   
            // Place a dummy component.
            m_engine->ComponentPlaceMode();
            // Remove popup.
            app.queuePopLayer(m_name);
        }
    }

    // ------------------- //
    //  C O M P O N E N T  //
    // ------------------- //

    else if (m_engine->m_activeComponent || m_engine->m_activeCable)
    {
        if (ImGui::MenuItem("Component Editor", "E"))
        {
            // Pushing this GUI layer defocuses the popup, causing a 
            // defocus event, which removes the popup event.
            app.pushGuiLayer<ComponentEditor>("Component Editor", DockPanel::Left);
        }
        if (ImGui::MenuItem("Color Editor"))
        {
            ColorEditor* editor = app.pushGuiLayer<ColorEditor>("Color Editor", DockPanel::Floating)->getGui();
            glm::vec2 localMousePos = getMousePosition();
            glm::vec2 pos = {

                localMousePos.x + m_contentRegionPosition.x,
                localMousePos.y + m_contentRegionPosition.y
            };
            editor->m_initialPosition = pos;
        }
        
        //if (ImGui::MenuItem("Edit Ports", "P"))
        //{
        //    // Remove popup.
        //    Lumen::getApp().queuePopLayer(m_name);
        //}
        /*if (ImGui::MenuItem("Add Cable", "C"))
        {
            //
            // Remove popup.
            app.queuePopLayer(m_name);
        }*/
        if (ImGui::MenuItem("Remove component", "DEL"))
        {
            m_engine->deleteActiveComponent();

            // Remove popup.
            app.queuePopLayer(m_name);
        }
    }
    ImGui::Separator();

    // --------------- //
    //  D E F A U L T  //
    // --------------- //

    if (ImGui::MenuItem("Circuit Editor..."))
    {
        CircuitEditor* editor = app.pushGuiLayer<CircuitEditor>("Circuit Editor", DockPanel::Right)->getGui();
        editor->setEngine(m_engine);
        editor->setActiveEngineTracking(true);
    }

    if (ImGui::MenuItem("Load Circuit...", "Ctrl+L"))
    {
        // Create and log load event.
        std::string path = selectFile("Lumen Load Circuit", "", "", "Load");
        if (path.size())
        {
            FileLoadEvent event(path);
            app.logEvent<FileLoadEvent>(event);
        }
        // Remove popup.
        app.queuePopLayer(m_name);
    }
    if (ImGui::MenuItem("Save Circuit...", "Ctrl+S"))
    {
        // Create and log save event.
        std::string path = selectFile("Lumen Save Circuit", "", m_engine->m_circuit->m_label, "Save");
        if (path.size())
        {
            FileSaveEvent event(path, m_engine);
            app.logEvent<FileSaveEvent>(event);
        }
        // Remove popup.
        app.queuePopLayer(m_name);
    }
}

void PopUpMenu::end() 
{
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

/*=======================================================================================================================================*/
/* EVents.																															     */
/*=======================================================================================================================================*/

void PopUpMenu::onDefocusEvent(LayerEvent& event) 
{
    Lumen::getApp().queuePopLayer(m_name);
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/
