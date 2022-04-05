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
#include "Engines/Design2DEngine/ComponentDesigner.h"
#include "Application/Events/EventLog.h"

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

void PopUpMenu::setInitialPosition(const glm::vec2& pos) 
{
    m_initialPos = pos;
}

void PopUpMenu::setEngine(Base2DEngine* engine)
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
    if (dynamic_cast<Design2DEngine*>(m_engine)) {
        Design2DEngine* design_engine = dynamic_cast<Design2DEngine*>(m_engine);
        // Render menu items.
        if (!design_engine->m_activeComponent && !design_engine->m_activeCable)
        {
            if (ImGui::MenuItem("Place component", "P"))
            {
                // Place a dummy component.
                design_engine->ComponentPlaceMode();
                // Remove popup.
                app.queuePopLayer(m_name);
            }
        }

        // ------------------- //
        //  C O M P O N E N T  //
        // ------------------- //

        else if (design_engine->m_activeComponent || design_engine->m_activeCable)
        {
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                // Pushing this GUI layer defocuses the popup, causing a 
                // defocus event, which removes the popup event.
                app.pushGuiLayer<ComponentEditor>("Component Editor", LumenDockPanel::Left);
            }
            if (ImGui::MenuItem("Color Editor"))
            {
                ColorEditor* editor = app.pushGuiLayer<ColorEditor>("Color Editor", LumenDockPanel::Floating)->getGui();
                glm::vec2 localMousePos = getMouseLocalPosition();
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
                design_engine->deleteActiveComponent();

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
            CircuitEditor* editor = app.pushGuiLayer<CircuitEditor>("Circuit Editor", LumenDockPanel::Right)->getGui();
            editor->setEngine(design_engine);
            editor->setActiveEngineTracking(true);
        }

        if (ImGui::MenuItem("Load Circuit...", "Ctrl+L"))
        {
            // Create and log load event.
            auto path = selectFile("Lumen Load Circuit", "", "", "Load");
            if (path.string().size())
            {
                EventLog::log<FileLoadEvent>(FileLoadEvent(path.string()));
            }
            // Remove popup.
            app.queuePopLayer(m_name);
        }
        if (ImGui::MenuItem("Save Circuit...", "Ctrl+S"))
        {
            // Create and log save event.
            auto path = selectFile("Lumen Save Circuit", "", design_engine->m_circuit->m_label, "Save");
            if (path.string().size())
            {
                EventLog::log<FileSaveEvent>(FileSaveEvent(path.string(), design_engine));
            }
            // Remove popup.
            app.queuePopLayer(m_layer);
        }
    }
    else if (dynamic_cast<ComponentDesigner*>(m_engine)) {
        Component2D* active_component = (dynamic_cast<ComponentDesigner*>(m_engine))->m_activeComponent.get();
        if (ImGui::MenuItem("Component Editor", "E"))
        {
            // Pushing this GUI layer defocuses the popup, causing a 
            // defocus event, which removes the popup event.
            app.pushGuiLayer<ComponentEditor>("Component Editor", LumenDockPanel::Left);
        }
        if (ImGui::MenuItem("Save Component...", "Ctrl+S"))
        {
            // Create and log save event.
            auto path = selectFile("Lumen Save Component", "",active_component->equipType, "Save");
            if (path.string().size())
            {
                EventLog::log<FileSaveEvent>(FileSaveEvent(path.string(), dynamic_cast<ComponentDesigner*>(m_engine)));
            }
            // Remove popup.
            app.queuePopLayer(m_name);
        }
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

void PopUpMenu::onDefocusEvent(NotifyEvent& event) 
{
    Lumen::getApp().queuePopLayer(m_layer);
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/
