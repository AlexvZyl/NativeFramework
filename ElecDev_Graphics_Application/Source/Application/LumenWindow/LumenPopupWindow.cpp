#include "LumenPopupWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Application/Events/Events.h"

LumenPopupWindow::LumenPopupWindow(const std::string& name, int imguiWindowFlags) 
	: LumenWindow(name, imguiWindowFlags)
{
    addImGuiWindowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);
    m_position = getMouseGlobalPosition();
}

void LumenPopupWindow::setPosition(const glm::vec2& position) 
{
    m_position = position;
}

void LumenPopupWindow::onImGuiBegin() 
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8.f, 5.f });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 6.f, 6.f });
    ImGui::SetNextWindowPos(m_position, ImGuiCond_Once);
    ImGui::Begin(getImGuiName(), &m_isOpen, getImGuiWindowFlags());
}

void LumenPopupWindow::onImGuiEnd()
{
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void LumenPopupWindow::onEvent(const Event& event) 
{
    if (event.isConsumed()) return;

    if (event.isType(EventType_Defocus)) onDefocusEvent(event.cast<NotifyEvent>());
}

void LumenPopupWindow::onDefocusEvent(const NotifyEvent& event) 
{
    closeWindow();
}