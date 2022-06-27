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
#include <algorithm>

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
	m_activeComponent->title->updateText(m_activeComponent->title->m_string);
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
		m_activePort = std::make_shared<Port>(getNearestGridVertex({ pixelToWorldCoords(getMouseLocalPosition()) }), PortType::PORT_INOUT, m_activeComponent.get());
		designerState = CompDesignState::PLACE_PORT;
		break;

	case CompDesignState::ADD_TEXT:
		// Add new text.
		switchState(CompDesignState::SELECT);
		designerState = CompDesignState::ADD_TEXT;
		break;

	case CompDesignState::SELECT:
		// Disable outlines.
		for(auto primitive : m_activePrimitives) primitive->disableOutline();
		if (m_activePort.get()) m_activePort->disableOutline();
		// Remove temp primitives.
		if (m_tempCircle) Renderer::remove(m_tempCircle);
		if (m_tempLine) Renderer::remove(m_tempLine);
		if (m_tempPoly) Renderer::remove(m_tempPoly);
		// Set state.
		m_activePrimitives.clear();
		m_tempCircle = nullptr;
		m_tempLine = nullptr;
		m_tempPoly = nullptr;
		m_activePort = nullptr;
		m_hoveredVertexIdx = -1;
		m_activeVertexIdx = -1;
		hoveredVertexOwner = nullptr;
		activeVertexOwner = nullptr;
		designerState = CompDesignState::SELECT;
		break;
	}
}

void ComponentDesigner::pushTempPrimitives()
{
	//TODO: get rid of these nasty dynamic casts!!!
	//This should perform checks to ensure that we are not pushing primitives multiple times
	//PolyLine* polyline = dynamic_cast<PolyLine*>(m_activePrimitive);
	//Polygon2D* polygon = dynamic_cast<Polygon2D*>(m_activePrimitive);
	//Circle* circle = dynamic_cast<Circle*>(m_activePrimitive);

	if (m_tempCircle) m_activeComponent->addCircle(m_tempCircle);

	//need an if/else here to differentiate (polylines will be successfilly cast to both PolyLine and Polygon2D).
	if (m_tempLine) m_activeComponent->addLine(m_tempLine);
	if (m_tempPoly) m_activeComponent->addPoly(m_tempPoly);
	// Set state.
	m_tempCircle = nullptr;
	m_tempLine = nullptr;
	m_tempPoly = nullptr;
}

void ComponentDesigner::setActivePrimitives(unsigned eID)
{
	if ((eID == 0) || (eID == -1)) { 
		// Remove outline of current entity.
		for (IPrimitive* primitive : m_activePrimitives) {
			primitive->disableOutline();
		}
		if (m_activePort.get()) m_activePort->disableOutline();

		// Remove previous selection.
		m_activePrimitives.clear();
		m_activePort = nullptr;
		}
	else
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return;

		//If we are clicking on an already selected target, nothing should happen
		if (std::find(m_activePrimitives.begin(), m_activePrimitives.end(), currentEntity) != m_activePrimitives.end()) return;

		//Otherwise, switch active primitives
		// Remove outline of current entity.
		for (IPrimitive* primitive : m_activePrimitives) {
			primitive->disableOutline();
		}
		if (m_activePort.get()) m_activePort->disableOutline();

		// Remove previous selection.
		m_activePrimitives.clear();
		m_activePort = nullptr;

		// Entity is a primitive belonging to the component.
		if (currentEntity->m_parent == m_activeComponent.get())
		{
			//We should only every get primitives. 
			LUMEN_DEBUG_ASSERT(currentEntity->m_type == EntityType::PRIMITIVE);
			m_activePrimitives.push_back(dynamic_cast<IPrimitive*>(currentEntity));
			m_activePrimitives.back()->enableOutline();
		}
		// Port.
		else if (dynamic_cast<Port*>(currentEntity->m_parent))
		{
			// Port text is selected.
			//TODO: Get rid of this dynamic cast to check types!!
			if (dynamic_cast<Text*>(currentEntity))
			{
				m_activePrimitives.push_back(dynamic_cast<IPrimitive*>(currentEntity));
				m_activePrimitives.back()->enableOutline();
			}
			// Port body is selected.
			else
			{
				//This is a bit hacky. 
				//TODO: Find a more elegant way of doing this
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

void ComponentDesigner::toggleActivePrimitives(unsigned eID)
{
	// Remove outline of current entity.
	//if (m_activePrimitive) m_activePrimitive->disableOutline();
	//if (m_activePort.get()) m_activePort->disableOutline();

	// Remove previous selection.
	//m_activePrimitive = nullptr;
	//m_activePort = nullptr;

	if ((eID == 0) || (eID == -1)) { return; }//ignore IDs that do not belong to the EntityManager
	else
	{
		Entity* currentEntity = EntityManager::getEntity(eID);
		if (!currentEntity) return;
		//First check to remove from active vector
		auto removeActive = std::find(m_activePrimitives.begin(), m_activePrimitives.end(), currentEntity);
		if (removeActive != m_activePrimitives.end()) {
			(*removeActive)->disableOutline();
			m_activePrimitives.erase(removeActive);
		}
		else {//add to active vector
		// Entity is a primitive belonging to the component.
			if (currentEntity->m_parent == m_activeComponent.get())
			{
				m_activePrimitives.push_back(dynamic_cast<IPrimitive*>(currentEntity));
				m_activePrimitives.back()->enableOutline();
			}
			// Port.
			else if (dynamic_cast<Port*>(currentEntity->m_parent))
			{
				// Port text is selected.
				//TODO: Get rid of this dynamic cast to check types!!
				if (dynamic_cast<Text*>(currentEntity))
				{
					m_activePrimitives.push_back(dynamic_cast<IPrimitive*>(currentEntity));
					m_activePrimitives.back()->enableOutline();
				}
				// Port body is selected. For now, we ignore this.
				/*
				else
				{
					//This is a bit hacky.
					//TODO: Find a more elegant way of doing this
					Port* cur = dynamic_cast<Port*>(currentEntity->m_parent);
					m_activePort = *std::find_if(begin(m_activeComponent->ports), end(m_activeComponent->ports), [&](std::shared_ptr<Port> current)
						{
							return current.get() == cur;
						});
					if (m_activePort) m_activePort->enableOutline();
				}*/
			}
		}
	}
}

void ComponentDesigner::setActiveVertex(glm::vec2 coords)
{
	//Currently this only works properly for Polygons and PolyLines. Other primitives will return the index of the underlying graphical vertices (not logical)
	//TODO: Fix this for other primitives
	m_activeVertexIdx = -1;
	for (IPrimitive* primitive : m_activePrimitives) {
		auto [vertexIdx, distance] = primitive->getNearestVertexIndex(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_activeVertexIdx = vertexIdx;
			activeVertexOwner = primitive;
			//we are only looking for one vertex, so we can set the active primitive and break here 
			//(in future, we could select the nearest vertex, but this will do for now)
			break;
		}
	}
}

void ComponentDesigner::setHoveredVertex(glm::vec2 coords)
{
	m_hoveredVertexIdx = -1;
	for(IPrimitive* primitive : m_activePrimitives)
	{
		auto [vertexIdx, distance] = primitive->getNearestVertexIndex(coords);
		if (worldToPixelDistance({ distance, 0.f, 0.f }).x < clickTol)
		{
			m_hoveredVertexIdx = vertexIdx;
			hoveredVertexOwner = primitive;
			//we only want one selected vertex at a time, so we can break here
			break;
		}
	}
}

void ComponentDesigner::deleteActivePrimitive()
{
	// Disable outline.
	if (m_activePort.get()) m_activePort->disableOutline();
	for (auto primitive : m_activePrimitives) {
		primitive->disableOutline();

		// Remove primitives.
		//TODO: Get rid of these dynamic casts!!!
		PolyLine* polyline = dynamic_cast<PolyLine*>(primitive);
		Polygon2D* polygon = dynamic_cast<Polygon2D*>(primitive);
		Circle* circle = dynamic_cast<Circle*>(primitive);
		Text* text = dynamic_cast<Text*>(primitive);

		if (polyline) m_activeComponent->removeLine(polyline);
		else if (polygon) m_activeComponent->removePoly(polygon);//This line will throw some warnings without the else (Polyline is a child of Polygon)
		if (circle) m_activeComponent->removeCircle(circle);
		if (m_activePort) m_activeComponent->removePort(m_activePort);
		if (text)
		{
			// If the text belongs to a port, we can delete the whole port.
			if (text->m_parent->m_type == EntityType::PORT)
				m_activeComponent->removePort(dynamic_cast<Port*>(text->m_parent));
			else m_activeComponent->removeText(text);
		}
	}

	//remove previous selection
	m_activePrimitives.clear();
	m_tempCircle = nullptr;
	m_tempLine = nullptr;
	m_tempPoly = nullptr;
	m_activePort = nullptr;
	m_hoveredVertexIdx = -1;
	m_activeVertexIdx = -1;
	hoveredVertexOwner = nullptr;
	activeVertexOwner = nullptr;
	//switchState(CompDesignState::SELECT);
}

//TODO: move this to a seperate file
void ComponentDesigner::renderOverlay()
{
	constexpr glm::vec2 button_size = { 35, 35 };
	constexpr glm::vec2 dropdown_size = { 10, 10 };

	//Add indicator for hovered vertices.
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (m_hoveredVertexIdx != -1) {

		glm::vec2 pos;
		if (hoveredVertexOwner) {
			pos = localToGlobalCoords(worldToPixelCoords(hoveredVertexOwner->getLogicalVertex(m_hoveredVertexIdx)));
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

		//Dynamic cast is probably unavoidable here - we use the result for text specific functions later on.
		std::vector<Text*> activeText;
		for (auto primitive : m_activePrimitives) {
			Text* text = dynamic_cast<Text*>(primitive);
			if (text) activeText.push_back(text);
		}
		if (!activeText.empty()) {
			buttonCol = activeText.at(0)->m_colour;//we only need one colour
		}
		if (designerState == CompDesignState::ADD_TEXT)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
			if (ImGui::ImageButton((void*)draw_text_icon, button_size, { 0, 1 }, { 1, 0 }, -1, { 0.f, 0.f, 0.f, 0.f }, buttonCol))
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
				if (!activeText.empty())
				{
					//we take the size of the first selected text
					int tempSizePt = (int)std::round(activeText.at(0)->m_textScale * 2835);
					if (ImGui::InputInt("pt ", &tempSizePt)) {
						for (auto text : activeText) {
							text->setScale(tempSizePt / 2835.f);
							// TODO: fire this as a batch command
							commandLog.execute<ChangeValueWithSetterCommand<float, Text>>(tempSizePt / 2835.f, text, text->m_textScale, &Text::setScale);
						}
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
				//Find a better way to set this width!
				ImGui::PushItemWidth(80.0f);
				if (!activeText.empty())
				{
					//we take the size of the first selected text
					int tempSizePt = (int)std::round(activeText.at(0)->m_textScale * 2835);
					if (ImGui::InputInt("pt ", &tempSizePt)) {
						for (auto text : activeText) {
							text->setScale(tempSizePt / 2835.f);
							// TODO: fire this as a batch command
							commandLog.execute<ChangeValueWithSetterCommand<float, Text>>(tempSizePt / 2835.f, text, text->m_textScale, &Text::setScale);
						}
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

		// TODO: check this sideways cast works.
		std::vector<HasThickness*> clearPrimitives;
		for (auto primitive : m_activePrimitives) {
			HasThickness* clearPrimitive = dynamic_cast<HasThickness*>(primitive);
			if (clearPrimitive) clearPrimitives.push_back(clearPrimitive);
		}

		if (!clearPrimitives.empty()) {
			float thickness = clearPrimitives.at(0)->getThickness();
			if (ImGui::SliderFloat("##Thickness", &thickness, 0.0001f, 0.005f, "%0.4f")) {
				for (auto clearPrimitive : clearPrimitives) {
					// TODO fire as batch command
					commandLog.execute<ChangeValueWithSetterCommand<float, HasThickness>>(thickness, clearPrimitive, clearPrimitive->getThickness(), &HasThickness::setThickness);
				}
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
		if (designerState == CompDesignState::SELECT && (!m_activePrimitives.empty() || m_activePort)) {
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
	/*m_activeComponent = nullptr;
	m_activePrimitives.clear();
	m_tempCircle = nullptr;
	m_tempLine = nullptr;
	m_tempPoly = nullptr;
	m_activePort = nullptr;
	m_activeVertexIdx = -1;
	*/
	switchState(CompDesignState::SELECT);
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
	glm::vec2 icon_size = { 30, 30 };
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
			ImGui::Image((void*)draw_filled_poly_icon, icon_size, { 0, 1 }, { 1, 0 }, penColour);
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