#include "ComponentDesigner.h"
#include "imgui/imgui.h"


ComponentDesigner::ComponentDesigner():Base2DEngine()
{
	m_activeComponent = std::make_shared<Component2D>(nullptr);
	m_activeComponent->place(glm::vec2(0.f));
	m_activeComponent->unhighlight();
	m_hasDesignPalette = true;
}

void ComponentDesigner::renderDesignPalette() 
{
	if (ImGui::MenuItem("Polygon", "P", &m_polygon))
	{
		clearStates();
		m_polygon = true;
	}

	ImGui::SameLine();
	ImGui::Separator();
	ImGui::SameLine();

	if (ImGui::MenuItem("Lines", "L", &m_lines))
	{
		clearStates();
		m_lines = true;
	}

	if(ImGui::BeginMenu("Line Settings..."))
	{
		ImGui::MenuItem("Test1");
		ImGui::MenuItem("Test2");
		ImGui::MenuItem("Test3");
		ImGui::EndMenu();
	}

	ImGui::SameLine();
	ImGui::Separator();
	ImGui::SameLine();

	if (ImGui::MenuItem("Ports", "", &m_lines))
	{
		clearStates();
		m_lines = true;
	}

	ImGui::SameLine();
	ImGui::Separator();
	ImGui::SameLine();

	if (ImGui::MenuItem("Delete", "Del", &m_delete))
	{
		clearStates();
		m_ports = true;
	}
}
