#include "ComponentDesigner.h"
#include "imgui/imgui.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Graphics/Entities/EntityManager.h"
#include "GUI/ComponentDesignerColorEditor.h"
#include "OpenGL/Primitives/Grid.h"

ComponentDesigner::ComponentDesigner()
	: Base2DEngine()
{
	m_activeComponent = std::make_shared<Component2D>(nullptr);
	m_activeComponent->place(glm::vec2(0.f));
	m_activeComponent->disableOutline();
	enableDesignPalette();
	getScene().getGrid().disableHelperCircle();
}

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
		m_activePort = std::make_shared<Port>(getNearestGridVertex( { pixelToWorldCoords(getMouseLocalPosition()) }), next_port_type, m_activeComponent.get());
		designerState = CompDesignState::PLACE_PORT;
		break;

	case CompDesignState::SELECT:
		if (designerState == CompDesignState::DRAW_CIRCLE || designerState == CompDesignState::DRAW_POLY || designerState == CompDesignState::DRAW_LINE) 
		{
			if (m_activeCircle) 
			{
				Renderer::remove(m_activeCircle);
			}
			if (m_activePoly) 
			{
				Renderer::remove(m_activePoly);
			}
			if (m_activeLine) 
			{
				Renderer::remove(m_activeLine);
			}
		}
		if (m_activeText) 
		{
			m_activeText->disableOutline();
		}
		if (m_activePort.get()) 
		{
			m_activePort->disableOutline();
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
	if (m_activeCircle) 
	{
		m_activeComponent->addCircle(m_activeCircle);
	}
	if (m_activePoly) 
	{
		m_activeComponent->addPoly(m_activePoly);
	}
	if (m_activeLine) 
	{
		m_activeComponent->addLine(m_activeLine);
	}
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
}

void ComponentDesigner::setActivePrimitives(unsigned eID)
{
	//Remove outline of current entity
	if (m_activePoly) 
	{
		m_activePoly->disableOutline();
	}
	if (m_activeLine) 
	{
		m_activeLine->disableOutline();
	}
	if (m_activeCircle) 
	{
		m_activeCircle->disableOutline();
	}
	if (m_activeText) 
	{
		m_activeText->disableOutline();
	}
	if (m_activePort.get()) 
	{
		m_activePort->disableOutline();
	}

	// Remove previous selection.
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
	m_activePort = nullptr;
	m_activeText = nullptr;

	if ((eID == 0) || (eID == -1))
	{
		Lumen::getApp().m_guiState->clickedZone.background = true;
	}
	else 
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (currentEntity->m_parent == m_activeComponent.get()) 
		{
			//Entity is a primitive belonging to the component
			if (dynamic_cast<LineSegment*>(currentEntity))
			{
				//Line
				m_activeLine = dynamic_cast<LineSegment*>(currentEntity);
				m_activeLine->enableOutline();
			}
			else if (dynamic_cast<Polygon2D*>(currentEntity)) 
			{
				//Polygon
				m_activePoly = dynamic_cast<Polygon2D*>(currentEntity);
				m_activePoly->enableOutline();
			}
			else if (dynamic_cast<Circle*>(currentEntity)) 
			{
				//Circle
				m_activeCircle = dynamic_cast<Circle*>(currentEntity);
				m_activeCircle->enableOutline();
			}
			else if (dynamic_cast<Text*>(currentEntity)) 
			{
				//Text
				m_activeText = dynamic_cast<Text*>(currentEntity);
				m_activeText->enableOutline();//We may want to rather outline the textbox in the future or change how text is highlighted.
			}
		}
		else if (dynamic_cast<Port*>(currentEntity->m_parent)) 
		{
			//Port
			if (dynamic_cast<Text*>(currentEntity)) 
			{
				//Port text is selected
				m_activeText = dynamic_cast<Text*>(currentEntity);
				m_activeText->enableOutline();//We may want to rather outline the textbox in the future or change how text is highlighted.
			}
			else 
			{
				// Port body is selected.
				Port* cur = dynamic_cast<Port*>(currentEntity->m_parent);
				m_activePort = *std::find_if(begin(m_activeComponent->ports), end(m_activeComponent->ports), [&](std::shared_ptr<Port> current)
					{
						return current.get() == cur;
					});
				if (m_activePort) 
				{
					m_activePort->enableOutline();
				}
			}
		}
	}
}

void ComponentDesigner::setActiveVertex(glm::vec2 coords)
{
	m_activeVertex = nullptr;
	if (m_activePoly) 
	{
		auto [vertexPtr, distance] = m_activePoly->getNearestVertex(coords);
		if (distance < clickTol) 
		{
			m_activeVertex = vertexPtr;
		}
	}
	/*if (m_activeCircle) {
		auto [vertexPtr, distance] = m_activeCircle->getNearestVertex(coords);
		if (distance < clickTol) {
			m_activeVertex = vertexPtr;
		}
	}*/
	else if (m_activeLine) 
	{
		auto [vertexPtr, distance] = m_activeLine->getNearestVertex(coords);
		if (distance < clickTol) 
		{
			m_activeVertex = vertexPtr;
		}
	}
}

void ComponentDesigner::deleteActivePrimitive()
{

	if (m_activeLine)
	{
		m_activeComponent->removeLine(m_activeLine);
	}
	if (m_activePoly) 
	{
		m_activeComponent->removePoly(m_activePoly);
	}
	if (m_activeCircle) 
	{
		m_activeComponent->removeCircle(m_activeCircle);
	}
	if (m_activePort) 
	{
		m_activeComponent->removePort(m_activePort);
	}

	//remove previous selection
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
	m_activePort = nullptr;
	m_activeVertex = nullptr;
	m_activeText = nullptr;
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

	if (ImGui::MenuItem("Color Editor"))
	{
		Lumen::getApp().pushWindow<ComponentDesignerColorEditor>(LumenDockPanel::Floating, "Color Editor")->setInitialPosition(getMouseGlobalPosition());
	}
}

void ComponentDesigner::setComponent(const std::filesystem::path& path, Circuit* parent)
{
	m_activeComponent.reset();
	m_activePoly = nullptr;
	m_activeLine = nullptr;
	m_activeCircle = nullptr;
	m_activeText = nullptr;
	m_activePort.reset();
	m_activeComponent = std::make_shared<Component2D>(path, parent);
	m_activeComponent->disableOutline();
}