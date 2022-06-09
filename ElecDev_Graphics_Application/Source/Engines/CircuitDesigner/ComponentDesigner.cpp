#include "ComponentDesigner.h"
#include "imgui/imgui.h"
#include "Lumen.h"
#include "Application/Application.h"
#include "Graphics/OpenGL/Primitives/Polygon.h"
#include "Graphics/OpenGL/Primitives/PolyLine.h"
#include "Graphics/OpenGL/Primitives/LineSegment.h"
#include "Graphics/OpenGL/Primitives/Circle.h"
#include "Graphics/Entities/EntityManager.h"
#include "GUI/ComponentDesignerColorEditor.h"
#include "OpenGL/Primitives/Grid.h"
#include "OpenGL/SceneGL.h"
#include "Peripherals/Component2D.h"
#include "Application/ApplicationTemplates.h"
#include "Resources/ResourceHandler.h"

ComponentDesigner::ComponentDesigner()
	: Base2DEngine()
{
	m_activeComponent = std::make_shared<Component2D>(nullptr);
	m_activeComponent->title->updateText(m_activeComponent->equipType);
	m_activeComponent->place(glm::vec2(0.f));
	m_activeComponent->disableOutline();
	//enableMenuBar();
	enableOverlay();
	enableTooltip();
	getScene().getGrid()
		.disableHelperCircle()
		.setMajorGrid(GridUnit::MILLIMETER, 5);
	getScene().getCamera().scale2D(100.f);

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

	case CompDesignState::ADD_TEXT:
		//Add new text
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::ADD_TEXT;
		break;
	case CompDesignState::SELECT:

		if (m_activeText)
		{
			m_activeText->disableOutline();
		}
		if (m_activePoly)
		{
			m_activePoly->disableOutline();
		}
		if (m_activeLine)
		{
			m_activeLine->disableOutline();
		}
		if (m_activePort.get())
		{
			m_activePort->disableOutline();
		}

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

		m_activeLine = nullptr;
		m_activeText = nullptr;
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
		PolyLine* polyline = dynamic_cast<PolyLine*>(m_activePoly);
		if (polyline) {
			m_activeComponent->addLine(polyline);
		}
		else {
			m_activeComponent->addPoly(m_activePoly);
		}
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
		//Lumen::getApp().m_guiState->clickedZone.background = true;
	}
	else 
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (currentEntity->m_parent == m_activeComponent.get()) 
		{
			//Entity is a primitive belonging to the component
			if (dynamic_cast<PolyLine*>(currentEntity))
			{
				//Line
				m_activeLine = dynamic_cast<PolyLine*>(currentEntity);
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
	m_activeVertexIdx = -1;
	if (m_activePoly) 
	{
		auto [vertexIdx, distance] = m_activePoly->getNearestVertexIdx(coords);
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
		auto [vertexIdx, distance] = m_activeLine->getNearestVertexIdx(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_activeVertexIdx = vertexIdx;
		}
	}
}

void ComponentDesigner::deleteActivePrimitive()
{

	if (m_activeText)
	{
		m_activeText->disableOutline();
	}
	if (m_activePoly)
	{
		m_activePoly->disableOutline();
	}
	if (m_activeLine)
	{
		m_activeLine->disableOutline();
	}
	if (m_activePort.get())
	{
		m_activePort->disableOutline();
	}

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
	if (m_activeText)
	{
		//if the text belongs to a port, we can delete the whole port
		if (m_activeText->m_parent->m_type == EntityType::PORT) 
		{
			m_activeComponent->removePort(dynamic_cast<Port*>(m_activeText->m_parent));
		}
		m_activeComponent->removeText(m_activeText);
	}

	//remove previous selection
	m_activeLine = nullptr;
	m_activePoly = nullptr;
	m_activeCircle = nullptr;
	m_activePort = nullptr;
	m_activeVertexIdx = -1;
	m_activeText = nullptr;
}

void ComponentDesigner::renderOverlay() 
{
	constexpr glm::vec2 button_size = { 35, 35 };
	constexpr glm::vec2 dropdown_size = { 10, 10 };
	
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
		if (designerState == CompDesignState::ADD_TEXT) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_text_icon, button_size, { 0, 1 }, { 1, 0 }, -1, {0.f, 0.f, 0.f, 0.f}, textColour))
			{
				switchState(CompDesignState::SELECT);
			}
			ImGui::PopStyleColor(3);
		}
		else 
		{
			if (ImGui::ImageButton((void*)draw_text_icon, button_size, { 0, 1 }, { 1, 0 }, -1, { 0.f, 0.f, 0.f, 0.f }, textColour))
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
			if (ImGui::BeginButtonDropDown("##TextSettings", { 0.f, button_size.y + 7.f })) {
				ImGui::PopStyleColor(3);
				if (ImGui::MenuItemEx(ICON_FA_PALETTE, "Colour")) {
					Lumen::getApp().pushWindow<ComponentDesignerColorEditor>(LumenDockPanel::Floating, "Color Editor", 0, &textColour)->setInitialPosition(getMouseGlobalPosition());
				}
				ImGui::Text("Size");
				ImGui::SameLine();
				//Find a better way to set this width
				ImGui::PushItemWidth(80.0f);
				if (ImGui::InputInt("pt ", &sizePt)) {
					textSize = sizePt / 2835.f;
				}
				ImGui::PopItemWidth();
				ImGui::EndButtonDropDown();
			}
			else{
				ImGui::PopStyleColor(3);
			}
		}
		else {
			if (ImGui::BeginButtonDropDown("##TextSettings", { 0.f, button_size.y + 7.f })) {
				if (ImGui::MenuItemEx(ICON_FA_PALETTE, "Colour")) {
					Lumen::getApp().pushWindow<ComponentDesignerColorEditor>(LumenDockPanel::Floating, "Color Editor", 0, &textColour)->setInitialPosition(getMouseGlobalPosition());
				}
				ImGui::Text("Size");
				ImGui::SameLine();
				//Find a better way to set this width
				ImGui::PushItemWidth(80.0f);
				if (ImGui::InputInt("pt ", &sizePt)) {
					textSize = sizePt/2835.f;
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
	m_activeComponent = std::make_shared<Component2D>(path, parent);
	m_activeComponent->disableOutline();
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