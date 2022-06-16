/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "PopUpMenu.h"
#include <iostream>
#include "imgui/imgui.h"
#include "Utilities/Windows/WindowsUtilities.h"
#include "Utilities/Serialisation/Serialiser.h"
#include "Engines/CircuitDesigner/CircuitDesigner.h"
#include "Engines/CircuitDesigner/Peripherals/Circuit.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "GUI/ComponentEditor/ComponentEditor.h"
#include "GUI/CircuitEditor/CircuitEditor.h"
#include "Engines/CircuitDesigner/ComponentDesigner.h"
#include "Application/Events/EventLog.h"
#include "Application/ApplicationTemplates.h"

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

PopUpMenu::PopUpMenu(const std::string& name, int imguiWindowFlags)
    : LumenPopupWindow(name, imguiWindowFlags)
{}

/*=======================================================================================================================================*/
/* Rendering.																															 */
/*=======================================================================================================================================*/

// Render call.
void PopUpMenu::onImGuiRender()
{ 
    Application& app = Lumen::getApp();
    CircuitDesigner* circuitEngine = app.getActiveEngine<CircuitDesigner>();
    ComponentDesigner* componentEngine = app.getActiveEngine<ComponentDesigner>();

    // --------------------- //
    //  B A C K G R O U N D  //
    // --------------------- //

    if (circuitEngine) 
    {
        // ------------------- //
        //  C O M P O N E N T  //
        // ------------------- //

        if (circuitEngine->m_activeComponent || circuitEngine->m_activeCable)
        {
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                app.pushWindow<ComponentEditor>(LumenDockPanel::Left, "Component Editor");
            }

            if (ImGui::MenuItem("Remove component", "DEL"))
            {
                if (circuitEngine->m_activeComponent)
                    circuitEngine->deleteActiveComponent();
                else if (circuitEngine->m_activeCable)
                    circuitEngine->deleteActiveCable();
                closeWindow();
            }

            ImGui::Separator();
        }

        // --------------- //
        //  D E F A U L T  //
        // --------------- //

        if (ImGui::MenuItem("Circuit Editor..."))
        {
            CircuitEditor* editor = app.pushWindow<CircuitEditor>(LumenDockPanel::Right, "Circuit Editor");
        }

        ImGui::Separator();

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
            EventLog::log<FileSaveEvent>(FileSaveEvent());
            closeWindow();
        }
    }
    else if (componentEngine)
    {
        if (componentEngine->designerState == CompDesignState::SELECT)
        {
            Component2D* active_component = (dynamic_cast<ComponentDesigner*>(componentEngine))->m_activeComponent.get();
            if (ImGui::MenuItem("Component Editor", "E"))
            {
                app.pushWindow<ComponentEditor>(LumenDockPanel::Left, "Component Editor");
            }
            if (componentEngine->m_activeCircle || componentEngine->m_activePoly || componentEngine->m_activeLine || componentEngine->m_activePort || componentEngine->m_activeText) 
            {
                if (ImGui::MenuItem("Delete", "DEL"))
                {
                    componentEngine->deleteActivePrimitive();
                    closeWindow();
                }
            }
            if (ImGui::MenuItem("Add Polygon", "P"))
            {
                componentEngine->switchState(CompDesignState::DRAW_POLY);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Line", "L"))
            {
                componentEngine->switchState(CompDesignState::DRAW_LINE);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Circle", "C"))
            {
                componentEngine->switchState(CompDesignState::DRAW_CIRCLE);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Port", "O"))
            {
                componentEngine->switchState(CompDesignState::PLACE_PORT);
                closeWindow();
            }
            if (ImGui::MenuItem("Add Text", "T"))
            {
                componentEngine->switchState(CompDesignState::ADD_TEXT);
                closeWindow();
            }
            if (ImGui::MenuItem("Save Component...", "Ctrl+S"))
            {
                // Create and log save event.
                EventLog::log<FileSaveEvent>();
                closeWindow();
            }
        }
        else 
        {
            if (ImGui::MenuItem("Done"))
            {
                componentEngine->pushActivePrimitives();
                componentEngine->switchState(CompDesignState::SELECT);
                closeWindow();
            }
            if (ImGui::MenuItem("Cancel", "ESC"))
            {
                componentEngine->switchState(CompDesignState::SELECT);
                closeWindow();
            }
        }
    }
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/
