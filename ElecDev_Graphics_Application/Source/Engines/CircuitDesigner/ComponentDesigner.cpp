#include "ComponentDesigner.h"
#include "imgui/imgui.h"
#include "Lumen/Lumen.h"
#include "Application/Application.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Graphics/OpenGL/Primitives/Text.h"
#include "Graphics/Entities/EntityManager.h"
#include "GUI/ComponentDesignerColorEditor.h"
#include "OpenGL/Primitives/Grid.h"
#include "OpenGL/SceneGL.h"
#include "Peripherals/Component2D.h"
#include "Application/ApplicationTemplates.h"
#include "Resources/ResourceHandler.h"
#include "Peripherals/Port.h"

ComponentDesigner::ComponentDesigner()
	: Base2DEngine()
{
	// Engine setup.
	enableOverlay();
	enableTooltip();
	getScene().getGrid()
		.disableHelperCircle()
		.setMajorGrid(GridUnit::MILLIMETER, 5);
	getScene().getCamera().scale2D(100.f);

	// Create default component.
	m_activeComponent = std::make_shared<Component2D>(nullptr);
	m_activeComponent->title->updateText(m_activeComponent->equipType);
	m_activeComponent->place(glm::vec2(0.f));
	m_activeComponent->disableOutline();

	// Load tooltip icons.
	draw_clear_poly_icon = loadBitmapToGL(loadImageFromResource(DRAW_POLY_CLEAR_ICON));
	draw_filled_poly_icon = loadBitmapToGL(loadImageFromResource(DRAW_POLY_FILLED_ICON));
	draw_clear_circle_icon = loadBitmapToGL(loadImageFromResource(DRAW_CIRCLE_CLEAR_ICON));
	draw_filled_circle_icon = loadBitmapToGL(loadImageFromResource(DRAW_CIRCLE_FILLED_ICON));
	draw_text_icon = loadBitmapToGL(loadImageFromResource(DRAW_TEXT_ICON));
	draw_line_icon = loadBitmapToGL(loadImageFromResource(DRAW_LINE_ICON));
	delete_icon = loadBitmapToGL(loadImageFromResource(DELETE_ICON));
	port_icon = loadBitmapToGL(loadImageFromResource(PORT_ICON));
	colour_palette_icon = loadBitmapToGL(loadImageFromResource(COLOUR_PALETTE_ICON));
	dropdown_icon = loadBitmapToGL(loadImageFromResource(DROPDOWN_ICON));
	pencil_icon = loadBitmapToGL(loadImageFromResource(PENCIL_ICON));
}

void ComponentDesigner::switchState(CompDesignState state)
{
	switch (state)
	{
	case CompDesignState::DRAW_POLY:
		// Add new polygon.
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::DRAW_POLY;
		break;

	case CompDesignState::DRAW_LINE:
		// Add new line.
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::DRAW_LINE;
		break;

	case CompDesignState::DRAW_CIRCLE:
		// Add new circle.
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::DRAW_CIRCLE;
		break;

	case CompDesignState::PLACE_PORT:
		// Add new port.
		switchState(CompDesignState::SELECT);
		m_activePort = std::make_shared<Port>(getNearestGridVertex( { pixelToWorldCoords(getMouseLocalPosition()) }), PortType::PORT_INOUT, m_activeComponent.get());
		designerState = CompDesignState::PLACE_PORT;
		break;

	case CompDesignState::ADD_TEXT:
		// Add new text.
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::ADD_TEXT;
		break;

	case CompDesignState::SELECT:
		// Disable outlines.
		if (m_activeText) m_activeText->disableOutline();
		if (m_activePoly) m_activePoly->disableOutline();
		if (m_activeLine) m_activeLine->disableOutline();
		if (m_activePort.get()) m_activePort->disableOutline();
		// Remove primitives.
		if (designerState == CompDesignState::DRAW_CIRCLE || designerState == CompDesignState::DRAW_POLY || designerState == CompDesignState::DRAW_LINE) 
		{
			if (m_activeCircle) Renderer::remove(m_activeCircle);
			if (m_activePoly) Renderer::remove(m_activePoly);
			if (m_activeLine) Renderer::remove(m_activeLine);
		}
		// Set state.
		m_activeLine = nullptr;
		m_activeText = nullptr;
		m_activePoly = nullptr;
		m_activeCircle = nullptr;
		m_activePort = nullptr;
		m_activeVertexIdx = -1;
		designerState = CompDesignState::SELECT;
		break;
	}
}

void ComponentDesigner::pushActivePrimitives()
{
	if (m_activeLine) m_activeComponent->addLine(m_activeLine);
	if (m_activeCircle) m_activeComponent->addCircle(m_activeCircle);
	if (m_activePoly) 
	{
		PolyLine* polyline = dynamic_cast<PolyLine*>(m_activePoly);
		if (polyline) m_activeComponent->addLine(polyline);
		else		  m_activeComponent->addPoly(m_activePoly);
	}
	// Set state.
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
}

void ComponentDesigner::setActivePrimitives(unsigned eID)
{
	// Remove outline of current entity.
	if (m_activePoly) m_activePoly->disableOutline();
	if (m_activeLine) m_activeLine->disableOutline();
	if (m_activeCircle) m_activeCircle->disableOutline();
	if (m_activeText) m_activeText->disableOutline();
	if (m_activePort.get()) m_activePort->disableOutline();

	// Remove previous selection.
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
	m_activePort = nullptr;
	m_activeText = nullptr;

	if ((eID == 0) || (eID == -1)) { }
	else 
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return;
		// Entity is a primitive belonging to the component.
		if (currentEntity->m_parent == m_activeComponent.get()) 
		{
			// Line.
			if (dynamic_cast<PolyLine*>(currentEntity))
			{
				m_activeLine = dynamic_cast<PolyLine*>(currentEntity);
				m_activeLine->enableOutline();
			}
			// Polygon.
			else if (dynamic_cast<Polygon2D*>(currentEntity)) 
			{
				m_activePoly = dynamic_cast<Polygon2D*>(currentEntity);
				m_activePoly->enableOutline();
			}
			// Circle.
			else if (dynamic_cast<Circle*>(currentEntity)) 
			{
				m_activeCircle = dynamic_cast<Circle*>(currentEntity);
				m_activeCircle->enableOutline();
			}
			// Text.
			else if (dynamic_cast<Text*>(currentEntity)) 
			{
				m_activeText = dynamic_cast<Text*>(currentEntity);
				m_activeText->enableOutline();
			}
		}
		// Port.
		else if (dynamic_cast<Port*>(currentEntity->m_parent)) 
		{
			// Port text is selected.
			if (dynamic_cast<Text*>(currentEntity)) 
			{
				m_activeText = dynamic_cast<Text*>(currentEntity);
				m_activeText->enableOutline();
			}
			// Port body is selected.
			else 
			{
				Port* cur = dynamic_cast<Port*>(currentEntity->m_parent);
				m_activePort = *std::find_if(begin(m_activeComponent->ports), end(m_activeComponent->ports), [&](std::shared_ptr<Port> current)
					{
						return current.get() == cur;
					});
				if (m_activePort) m_activePort->enableOutline();
			}
		}
	}
}

void ComponentDesigner::setActiveVertex(glm::vec2 coords)
{
	m_activeVertexIdx = -1;
	if (m_activePoly) 
	{
		auto [vertexIdx, distance] = m_activePoly->getNearestVertexIndex(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_activeVertexIdx = vertexIdx;
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
		auto [vertexIdx, distance] = m_activeLine->getNearestVertexIndex(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_activeVertexIdx = vertexIdx;
		}
	}
}

void ComponentDesigner::setHoveredVertex(glm::vec2 coords)
{
	m_hoveredVertexIdx = -1;
	if (m_activePoly)
	{
		auto [vertexIdx, distance] = m_activePoly->getNearestVertexIndex(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_hoveredVertexIdx = vertexIdx;
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
		auto [vertexIdx, distance] = m_activeLine->getNearestVertexIndex(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_hoveredVertexIdx = vertexIdx;
		}
	}
}

void ComponentDesigner::deleteActivePrimitive()
{
	// Disable outline.
	if (m_activeText) m_activeText->disableOutline();
	if (m_activePoly) m_activePoly->disableOutline();
	if (m_activeLine) m_activeLine->disableOutline();
	if (m_activePort.get()) m_activePort->disableOutline();

	// Remove primitive.
	if (m_activeLine) m_activeComponent->removeLine(m_activeLine);
	if (m_activePoly) m_activeComponent->removePoly(m_activePoly);
	if (m_activeCircle) m_activeComponent->removeCircle(m_activeCircle);
	if (m_activePort) m_activeComponent->removePort(m_activePort);
	if (m_activeText)
	{
		// If the text belongs to a port, we can delete the whole port.
		if (m_activeText->m_parent->m_type == EntityType::PORT) 
			m_activeComponent->removePort(dynamic_cast<Port*>(m_activeText->m_parent));
		m_activeComponent->removeText(m_activeText);
	}

	//remove previous selection
	m_activePoly = nullptr;
	m_activeLine = nullptr;
	m_activeCircle = nullptr;
	m_activeText = nullptr;
	m_activePort = nullptr;
	m_activeVertexIdx = -1;
}

void ComponentDesigner::renderOverlay() 
{
	constexpr glm::vec2 button_size = { 35, 35 };
	constexpr glm::vec2 dropdown_size = { 10, 10 };

	//Add indicator for hovered vertices.
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (m_hoveredVertexIdx != -1) {

		glm::vec2 pos;
		if (m_activePoly) {
			pos = localToGlobalCoords(worldToPixelCoords(m_activePoly->getVertex(m_hoveredVertexIdx).position));
			pos = { pos.x, m_parentWindow->getMainViewportSize().y - pos.y };
		}
		if (m_activeLine) {
			pos = localToGlobalCoords(worldToPixelCoords(m_activeLine->m_vertices.at(m_hoveredVertexIdx)));
			pos = { pos.x, m_parentWindow->getMainViewportSize().y - pos.y };
		}
		draw_list->AddCircleFilled(pos, clickTol, ImColor(helperColour));
	}

	if (ImGui::BeginChild("##designPalette", { 0.f, button_size.y + 8.f }, true, ImGuiWindowFlags_AlwaysUseWindowPadding)) 
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.f);

		//ImGui::SetWindowSize({ -1.f, button_size.y });
		//ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { -1.f, -1.f });

		static float b = 0.8f; //  Temp colours. We should use standardised theme colours here.
		static float c = 0.5f; 
		static float i = 4.4f;

		if (designerState == CompDesignState::DRAW_POLY && !drawFilled) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_clear_poly_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_clear_poly_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::DRAW_POLY);
				drawFilled = false;
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Clear Polygon");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();


		if (designerState == CompDesignState::DRAW_POLY && drawFilled) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_filled_poly_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_filled_poly_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::DRAW_POLY);
				drawFilled = true;
			}
		}
		

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Filled Polygon");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();


		if (designerState == CompDesignState::DRAW_CIRCLE && !drawFilled) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_clear_circle_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_clear_circle_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::DRAW_CIRCLE);
				drawFilled = false;
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Clear Circle");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		if (designerState == CompDesignState::DRAW_CIRCLE && drawFilled) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_filled_circle_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_filled_circle_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::DRAW_CIRCLE);
				drawFilled = true;
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Filled Circle");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		if (designerState == CompDesignState::DRAW_LINE) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_line_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_line_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::DRAW_LINE);
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Line");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		if (designerState == CompDesignState::PLACE_PORT) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)port_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)port_icon, button_size, { 0, 1 }, { 1, 0 }))
			{
				switchState(CompDesignState::PLACE_PORT);
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Port");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		glm::vec4 buttonCol = textColour;
		if (m_activeText) {
			buttonCol = m_activeText->m_colour;
		}
		if (designerState == CompDesignState::ADD_TEXT) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_text_icon, button_size, { 0, 1 }, { 1, 0 }, -1, {0.f, 0.f, 0.f, 0.f}, buttonCol))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_text_icon, button_size, { 0, 1 }, { 1, 0 }, -1, { 0.f, 0.f, 0.f, 0.f }, buttonCol))
			{
				switchState(CompDesignState::ADD_TEXT);
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Text");
			ImGui::EndTooltip();
		}

		if (designerState == CompDesignState::ADD_TEXT) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::BeginButtonDropDown("##TextSettings", { 0.f, button_size.y + 7.f })) 
			{
				ImGui::PopStyleColor(3);
				if (ImGui::MenuItemEx(ICON_FA_PALETTE, "Colour")) 
				{
					Lumen::getApp().pushWindow<ComponentDesignerColorEditor>(LumenDockPanel::Floating, "Color Editor", 0, &textColour)->setInitialPosition(getMouseGlobalPosition());
				}
				ImGui::Text("Size");
				ImGui::SameLine();
				//Find a better way to set this width
				ImGui::PushItemWidth(80.0f);
				if (m_activeText) 
				{
					int tempSizePt = (int)std::round(m_activeText->m_textScale * 2835);
					if (ImGui::InputInt("pt ", &tempSizePt)) {
						m_activeText->setScale(tempSizePt / 2835.f);
					}
				}
				else if (ImGui::InputInt("pt ", &sizePt)) 
				{
					textSize = sizePt / 2835.f;
				}
				ImGui::PopItemWidth();
				ImGui::EndButtonDropDown();
			}
			else
			{
				ImGui::PopStyleColor(3);
			}
		}
		else {
			if (ImGui::BeginButtonDropDown("##TextSettings", { 0.f, button_size.y + 7.f })) 
			{
				if (ImGui::MenuItemEx(ICON_FA_PALETTE, "Colour")) 
				{
					Lumen::getApp().pushWindow<ComponentDesignerColorEditor>(LumenDockPanel::Floating, "Color Editor", 0, &textColour)->setInitialPosition(getMouseGlobalPosition());
				}
				ImGui::Text("Size");
				ImGui::SameLine();
				//Find a better way to set this width
				ImGui::PushItemWidth(80.0f);
				if (m_activeText) {
					int tempSizePt = (int)std::round(m_activeText->m_textScale * 2835);
					if (ImGui::InputInt("pt ", &tempSizePt)) {
						m_activeText->setScale(tempSizePt / 2835.f);
					}
				}
				else if (ImGui::InputInt("pt ", &sizePt)) {
					textSize = sizePt / 2835.f;
				}
				ImGui::PopItemWidth();
				ImGui::EndButtonDropDown();
			}
		}
		

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Text Settings");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		ImGui::Separator();
		ImGui::SameLine();

		if (ImGui::ImageButton((void*)pencil_icon, button_size, { 0, 1 }, { 1, 0 }, -1, { 0.f, 0.f, 0.f, 0.f }, penColour))
		{
			Lumen::getApp().pushWindow<ComponentDesignerColorEditor>(LumenDockPanel::Floating, "Color Editor", 0, &penColour)->setInitialPosition(getMouseGlobalPosition());
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Pen Colour");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();

		//Find a better way to set this width
		ImGui::PushItemWidth(100.0f);
		//FIXME
		ImGui::SetCursorPosY(16.f);
		if (m_activeLine) {
			float thickness = m_activeLine->m_thickness;
			if (ImGui::SliderFloat("##Thickness", &thickness, 0.0001f, 0.005f, "%0.4f")) {
				m_activeLine->setThickness(thickness);
			}
		}
		else if(m_activeCircle) {
			float thickness = m_activeCircle->m_thickness;
			if (ImGui::SliderFloat("##Thickness", &thickness, 0.0001f, 0.005f, "%0.4f")) {
				m_activeCircle->setThickness(thickness);
			}
		}
		else ImGui::SliderFloat("##Thickness", &penThickness, 0.0001f, 0.005f, "%0.4f");
		ImGui::PopItemWidth();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Line Thickness");
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		if (ImGui::GetCursorPosX() < ImGui::GetWindowContentRegionMax().x - 46)
			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 46);

		glm::vec4 delete_tint = { 0.2f, 0.2f, 0.2f, 1.0f };
		if (designerState == CompDesignState::SELECT && (m_activeCircle || m_activeLine || m_activePoly || m_activeText || m_activePort)) {
			delete_tint = { 0.8f, 0.f, 0.f, 1.f };
		}

		if (ImGui::ImageButton((void*)delete_icon, button_size, { 0, 1 }, { 1, 0 }, -1, { 0.f, 0.f, 0.f, 0.f }, delete_tint))
		{
			if (designerState == CompDesignState::SELECT) deleteActivePrimitive();
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Delete");
			ImGui::EndTooltip();
		}
		
		//ImGui::PopStyleVar();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();

	// Grid size overlay.
	getScene().getGrid().renderOverlay();
}

void ComponentDesigner::setComponent(const std::filesystem::path& path, Circuit* parent)
{
	m_activeComponent = nullptr;
	m_activePoly = nullptr;
	m_activeLine = nullptr;
	m_activeCircle = nullptr;
	m_activeText = nullptr;
	m_activePort = nullptr;
	m_activeVertexIdx = -1;
	m_activeComponent = std::make_shared<Component2D>(path, parent);
	m_activeComponent->disableOutline();
	savePath = path;
}

void ComponentDesigner::renderTooltip() 
{
	bool disp_string = false;

	ImGui::PushStyleColor(ImGuiCol_Border, glm::vec4(1.0, 0.0, 0.0, 1.0));
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.f);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, glm::vec4(0.0, 0.0, 0.0, 0.0));
	ImGui::BeginTooltipEx(ImGuiTooltipFlags_None, ImGuiWindowFlags_AlwaysAutoResize);
	glm::vec2 icon_size = {30, 30};
	std::string toolString;
	switch (designerState) 
	{
	case CompDesignState::SELECT:
		toolString = "Select";
		disp_string = false;
		break;
	case CompDesignState::ADD_TEXT:
		toolString = ICON_FA_PEN;
		disp_string = false;
		ImGui::Image((void*)draw_text_icon, icon_size, { 0, 1 }, { 1, 0 }, textColour);
		break;
	case CompDesignState::DRAW_CIRCLE:
		disp_string = false;
		if (drawFilled) 
		{
			ImGui::Image((void*)draw_filled_circle_icon, icon_size, { 0, 1 }, { 1, 0 }, penColour);
		}
		else 
		{
			ImGui::Image((void*)draw_clear_circle_icon, icon_size, { 0, 1 }, { 1, 0 }, penColour);
		}
		break;
	case CompDesignState::DRAW_POLY:
		//toolString = "Place vertex";
		//toolString = ICON_FA_DRAW_POLYGON;
		if (drawFilled) 
		{
			ImGui::Image((void*)draw_filled_poly_icon, icon_size, {0, 1}, {1, 0}, penColour);
		}
		else 
		{
			ImGui::Image((void*)draw_clear_poly_icon, icon_size, { 0, 1 }, { 1, 0 }, penColour);
		}
		disp_string = false;
		break;
	case CompDesignState::DRAW_LINE:
		toolString = ICON_FA_PENCIL_ALT;
		disp_string = false;
		ImGui::Image((void*)draw_line_icon, icon_size, { 0, 1 }, { 1, 0 }, penColour);
		break;
	case CompDesignState::PLACE_PORT:
		//ImGui::Text("Select");
		break;

	}

	if (disp_string) ImGui::Text(toolString.c_str());
	ImGui::EndTooltip();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void ComponentDesigner::renderMenuBar() 
{

}