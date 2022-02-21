/*=======================================================================================================================================*/
/* Includes.																															 */
/*=======================================================================================================================================*/

#include "ComponentEditorPopup.h"
#include <iostream>
#include "imgui/imgui.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "GUI/ComponentEditor/ComponentEditor.h"

/*=======================================================================================================================================*/
/* PopUp Menu.																															 */
/*=======================================================================================================================================*/

ComponentEditorPopup::ComponentEditorPopup(std::string name, int imguiWindowFlags)
    : GuiElementCore(name, imguiWindowFlags)
{
    m_imguiWindowFlags |= ImGuiWindowFlags_NoDecoration
                       | ImGuiWindowFlags_NoMove
                       | ImGuiWindowFlags_NoDocking
                       | ImGuiWindowFlags_AlwaysAutoResize;
}

void ComponentEditorPopup::setPosition(const glm::vec2& pos)
{
    m_pos = pos;
}

void ComponentEditorPopup::begin()
{
    // Place at mouse position.
    ImGui::SetNextWindowPos(ImVec2(m_pos.x, m_pos.y));
    ImGui::Begin(m_name.c_str(), &m_isOpen, m_imguiWindowFlags);
}

void ComponentEditorPopup::setComponentEditor(ComponentEditor* editor)
{
    m_compEditor = editor;
}

/*=======================================================================================================================================*/
/* Rendering.																															 */
/*=======================================================================================================================================*/

// Render call.
void ComponentEditorPopup::onRender()
{
    Application& app = Lumen::getApp();


    if (ImGui::MenuItem("From"))
    {

    }
    if (ImGui::MenuItem("Size"))
    {

    }
    if (ImGui::MenuItem("Combine text"))
    {

    }
    if (ImGui::MenuItem("IF"))
    {

    }
 
}

void ComponentEditorPopup::end()
{
    ImGui::End();
}

/*=======================================================================================================================================*/
/* EVents.																															     */
/*=======================================================================================================================================*/

void ComponentEditorPopup::onDefocusEvent(LayerEvent& event)
{
    Lumen::getApp().queuePopLayer(m_name);
}

/*=======================================================================================================================================*/
/* EOF.																																	 */
/*=======================================================================================================================================*/
