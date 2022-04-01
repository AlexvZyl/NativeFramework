#include "ComponentDesigner.h"
#include "imgui/imgui.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"


void ComponentDesigner::switchState(CompDesignState state)
{
	switch (state)
	{
	case CompDesignState::DRAW_POLY:
		//Add new polygon
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::DRAW_POLY;
		break;

	case CompDesignState::DRAW_LINE:
		//Add new line
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::DRAW_LINE;
		break;

	case CompDesignState::DRAW_CIRCLE:
		//Add new circle
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::DRAW_CIRCLE;
		break;

	case CompDesignState::PLACE_PORT:
		//Add new port
		switchState(CompDesignState::SELECT);
		m_activePort = std::make_shared<Port>(getNearestGridVertex( { m_scene->pixelCoordsToWorldCoords(getMousePosition()) }), next_port_type, m_activeComponent.get());
		designerState = CompDesignState::PLACE_PORT;
		break;

	case CompDesignState::SELECT:
		if (designerState == CompDesignState::DRAW_CIRCLE || designerState == CompDesignState::DRAW_POLY || designerState == CompDesignState::DRAW_LINE) {
			if (m_activeCircle) {
				Renderer::remove(m_activeCircle);
			}
			if (m_activePoly) {
				Renderer::remove(m_activePoly);
			}
			if (m_activeLine) {
				Renderer::remove(m_activeLine);
			}
		}
		m_activeLine = nullptr;
		m_activePoly = nullptr;
		m_activeCircle = nullptr;
		m_activePort = nullptr;
		designerState = CompDesignState::SELECT;
		break;
	}

}

void ComponentDesigner::pushActivePrimitives()
{
	if (m_activeCircle) {
		m_activeComponent->addCircle(m_activeCircle);
	}
	if (m_activePoly) {
		m_activeComponent->addPoly(m_activePoly);
	}
	if (m_activeLine) {
		m_activeComponent->addLine(m_activeLine);
	}
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
}

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
