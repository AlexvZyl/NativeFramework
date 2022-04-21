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
#include "Engines/Design2DEngine/ComponentDesigner.h"
#include "Application/Events/EventLog.h"

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

PopUpMenu::PopUpMenu(std::string name, int imguiWindowFlags)
    : LumenWindow(name.c_str(), imguiWindowFlags)
{
    addImGuiWindowFlags( ImGuiWindowFlags_NoDecoration
                       | ImGuiWindowFlags_NoMove
                       | ImGuiWindowFlags_NoDocking);
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

void PopUpMenu::onImGuiBegin()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 5.f });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.f, 6.f });
    ImGui::SetNextWindowPos(ImVec2(m_initialPos.x, m_initialPos.y), ImGuiCond_Once);
    ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void PopUpMenu::onEvent(const Event& event) 
{
    if (event.isConsumed()) return;

    if (event.isType(EventType_Defocus)) onDefocusEvent(event.cast<NotifyEvent>());
}

/*=======================================================================================================================================*/
/* Rendering.																															 */
/*=======================================================================================================================================*/

// Render call.
void PopUpMenu::onImGuiRender()
{ 
    Application& app = Lumen::getApp();

    // --------------------- //
    //  B A C K G R O U N D  //
    // --------------------- //

    if (dynamic_cast<Design2DEngine*>(m_engine)) 
    {
        Design2DEngine* design_engine = dynamic_cast<Design2DEngine*>(m_engine);
        // Render menu items.
        if (!design_engine->m_activeComponent && !design_engine->m_activeCable)
        {
            if (ImGui::MenuItem("Place component", "P"))
            {
                // Place a dummy component.
                design_engine->ComponentPlaceMode();
                closeWindow();
            }
        }

        // ------------------- //
        //  C O M P O N E N T  //
        // ------------------- //

        else if (design_engine->m_activeComponent || design_engine->m_activeCable)
        {
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                app.pushWindow<ComponentEditor>(LumenDockPanel::Left, "Component Editor");
            }

            if (ImGui::MenuItem("Remove component", "DEL"))
            {
                if (design_engine->m_activeComponent)
                    design_engine->deleteActiveComponent();
                else if (design_engine->m_activeCable)
                    design_engine->deleteActiveCable();
                closeWindow();
            }
        }
        ImGui::Separator();

        // --------------- //
        //  D E F A U L T  //
        // --------------- //

        if (ImGui::MenuItem("Circuit Editor..."))
        {
            CircuitEditor* editor = app.pushWindow<CircuitEditor>(LumenDockPanel::Right, "Circuit Editor");
            editor->setEngine(design_engine);
            editor->setActiveEngineTracking(true);
        }

        if (ImGui::MenuItem("Load Circuit...", "Ctrl+L"))
        {
            // Create and log load event.
            auto path = selectFile("Lumen Load Circuit", "", "", "Load");
            if (path.string().size())
            {
                EventLog::log<FileLoadEvent>(FileLoadEvent(path.string(), EventType_Application));
            }
            closeWindow();
        }
        if (ImGui::MenuItem("Save Circuit...", "Ctrl+S"))
        {
            // Create and log save event.
            auto path = selectFile("Lumen Save Circuit", "", design_engine->m_circuit->m_label, "Save");
            if (path.string().size())
            {
                EventLog::log<FileSaveEvent>(FileSaveEvent(path.string(), design_engine, EventType_Application));
                design_engine->savedDocument();
            }
            closeWindow();
        }
    }
    else if (dynamic_cast<ComponentDesigner*>(m_engine)) 
    {
        ComponentDesigner* component_designer = dynamic_cast<ComponentDesigner*>(m_engine);

        if (component_designer->designerState == CompDesignState::SELECT) 
        {
            Component2D* active_component = (dynamic_cast<ComponentDesigner*>(m_engine))->m_activeComponent.get();
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                app.pushWindow<ComponentEditor>(LumenDockPanel::Left, "Component Editor");
            }
            if (component_designer->m_activeCircle || component_designer->m_activePoly || component_designer->m_activeLine || component_designer->m_activePort) {
                if (ImGui::MenuItem("Delete", "DEL"))
                {
                    component_designer->deleteActivePrimitive();
                    closeWindow();
                }
            }
            if (ImGui::MenuItem("Add Polygon", "P"))
            {
                component_designer->switchState(CompDesignState::DRAW_POLY);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Line", "L"))
            {
                component_designer->switchState(CompDesignState::DRAW_LINE);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Circle", "C"))
            {
                component_designer->switchState(CompDesignState::DRAW_CIRCLE);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Port", "O"))
            {
                component_designer->switchState(CompDesignState::PLACE_PORT);
                closeWindow();
            }
            if (ImGui::MenuItem("Save Component...", "Ctrl+S"))
            {
                // Create and log save event.
                auto path = selectFile("Lumen Save Component", "", active_component->equipType, "Save");
                if (path.string().size())
                {
                    EventLog::log<FileSaveEvent>(FileSaveEvent(path.string(), component_designer, EventType_Application));
                    component_designer->savedDocument();
                }
                closeWindow();
            }
        }
        else 
        {
            if (ImGui::MenuItem("Done"))
            {
                component_designer->pushActivePrimitives();
                component_designer->switchState(CompDesignState::SELECT);
                closeWindow();
            }
            if (ImGui::MenuItem("Cancel", "ESC"))
            {
                component_designer->switchState(CompDesignState::SELECT);
                closeWindow();
            }
        }
    }
}

void PopUpMenu::onImGuiEnd() 
{
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

/*=======================================================================================================================================*/
/* EVents.																															     */
/*=======================================================================================================================================*/

void PopUpMenu::onDefocusEvent(const NotifyEvent& event) 
{
    closeWindow();
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/
