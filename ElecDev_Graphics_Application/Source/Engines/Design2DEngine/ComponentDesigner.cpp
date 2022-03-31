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

	ImGui::Button("Lines");
	ImGui::Button("Polygon");

	ImGui::SameLine();
	ImGui::Separator();
	ImGui::SameLine();

	ImGui::Button("Delete");
}
